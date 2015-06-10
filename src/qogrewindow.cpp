#include "qogrewindow.h"
#include "ponesettings.h"

#if OGRE_VERSION >= ((2 << 16) | (0 << 8) | 0)
#include <Compositor/OgreCompositorManager2.h>
#endif

QOgreWindow::QOgreWindow(QWindow *parent)
    : QWindow(parent),
      isUpdatePending(false),
      isAnimating(false),
      oRoot(NULL),
      oWin(NULL),
      oCam(NULL),
      camMan(NULL),
      resConfig("resources/resources.cfg"),
      pluginConfig(Ogre::StringUtil::BLANK)
{
    setAnimating(true);
    installEventFilter(this);
    oBgColor = Ogre::ColourValue(0.0f, 0.5f, 1.0f);
}

QOgreWindow::~QOgreWindow()
{
    if (camMan) delete camMan;
    delete oRoot;
}

bool QOgreWindow::go()
{
    return true;
}

/*
In case any drawing surface backing stores (QRasterWindow or QOpenGLWindow) of Qt are supplied to this
class in any way we inform Qt that they will be unused.
*/
void QOgreWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void QOgreWindow::initialize()
{
    // Initialize Ogre like normal
#ifdef _MSC_VER
    oRoot = new Ogre::Root(Ogre::String("plugins" OGRE_BUILD_SUFFIX ".cfg"));
#else
    oRoot = new Ogre::Root(Ogre::String("plugins.cfg"));
#endif
    Ogre::ConfigFile oConf;

    oConf.load(resConfig);

    Ogre::ConfigFile::SectionIterator seci = oConf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }

    if (!oRoot->restoreConfig() || oRoot->showConfigDialog()) return;

    const Ogre::RenderSystemList& rsList = oRoot->getAvailableRenderers();

    Ogre::RenderSystem* rs = rsList[0];

    // This list setup search order for used render system
    Ogre::StringVector renderOrder;
#if PONY_PLATFORM == PLAT_WIN32
    renderOrder.push_back("Direct3D9");
    renderOrder.push_back("Direct3D11");
#endif
    renderOrder.push_back("OpenGL");
    renderOrder.push_back("OpenGL 3+");

    for (Ogre::StringVector::iterator iter = renderOrder.begin(); iter != renderOrder.end(); iter++)
    {
        for (Ogre::RenderSystemList::const_iterator it = rsList.begin(); it != rsList.end(); it++)
        {
            if ((*it)->getName().find(*iter) != Ogre::String::npos)
            {
                rs = *it;
                break;
            }
        }
        if (rs != NULL) break;
    }
    if (rs == NULL)
    {
        if (!oRoot->restoreConfig())
        {
            if (!oRoot->showConfigDialog())
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "[PoneNgine][Ogre3D] Abort render system configuration.","QOgreWindow::initialize");
            }
        }
    }

    // Settings size on windows will solve a lot of problems
    // VSync... why is this praised by developers again? Becuase I hate it in games.
    // Soon, I'll make a settings class for the in-game settings.
    QString dimensions = QString("%1 x %2").arg(this->width()).arg(this->height());
    rs->setConfigOption("Video Mode", dimensions.toStdString());
    rs->setConfigOption("Full Screen", "No");
    rs->setConfigOption("VSync", "No");
    oRoot->setRenderSystem(rs);
    oRoot->initialise(false);

    Ogre::NameValuePairList parameters;

    if (rs->getName().find("GL") <= rs->getName().size())
        parameters["currentGLContext"] = Ogre::String("false");

#if PONY_PLATFORM == PLAT_WIN32 || PONY_PLATFORM == PLAT_MACOSX
    parameters["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)(this->winId()));
    parameters["parentWindowHandle"]   = Ogre::StringConverter::toString((size_t)(this->winId()));
#else
    parameters["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(this->winId()));
    parameters["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(this->winId()));
#endif

#if PONY_PLATFORM == PLAT_MACOSX
    parameters["macAPI"] = "cocoa";
    parameters["macAPICocoaUseNSView"] = "true";
#endif

    oWin = oRoot->createRenderWindow("QOgreWindow Test | PoneNgine Version "+Ogre::String(PONENGINE_VERSION)+" Build "+Ogre::String(PONENGINE_BUILD), this->width(), this->height(), false, &parameters);
    oWin->setVisible(true);

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

#if OGRE_VERSION >= ((2 << 16) | (0 << 8) | 0)
    const size_t numThreads = std::max<int>(1, Ogre::PlatformInformation::getNumLogicalCores());
    Ogre::InstancingTheadedCullingMethod threadedCullingMethod = Ogre::INSTANCING_CULLING_SINGLETHREAD;
    if (numThreads > 1)threadedCullingMethod = Ogre::INSTANCING_CULLING_THREADED;
    oSceneMgr = oRoot->createSceneManager(Ogre::ST_GENERIC, numThreads, threadedCullingMethod);
#else
    oSceneMgr = oRoot->createSceneManager(Ogre::ST_GENERIC);
#endif

    oCam = oSceneMgr->createCamera("MainCamera");
    oCam->setPosition(Ogre::Vector3(0.0f,0.0f,10.0f));
    oCam->lookAt(Ogre::Vector3(0.0f,0.0f,-300.0f));
    oCam->setNearClipDistance(0.1f);
    oCam->setFarClipDistance(200.0f);
    camMan = new OgreQtBites::QtOgreSdkCameraMan(oCam);

#if OGRE_VERSION >= ((2 << 16) | (0 << 8) | 0)
    createCompositor();
#else
    Ogre::Viewport* oViewPort = oWin->addViewport(oCam);
    oViewPort->setBackgroundColour(oBgColor);
#endif

    oCam->setAspectRatio(Ogre::Real(oWin->getWidth()) / Ogre::Real(oWin->getHeight()));
    oCam->setAutoAspectRatio(true);

    createScene();

    oRoot->addFrameListener(this);
}

void QOgreWindow::createScene()
{
    // Put anything in here, I'll just put an ogre head here for testing purposes. Go crazy.

    oSceneMgr->setAmbientLight(Ogre::ColourValue(0.5,0.5,0.5));
    Ogre::Entity* ogreHead = oSceneMgr->createEntity("ogrehead.mesh");
    Ogre::SceneNode* node = oSceneMgr->getRootSceneNode()->createChildSceneNode();

    node->attachObject(ogreHead);

    Ogre::Light* light = oSceneMgr->createLight("MainLight");
    light->setPosition(20, 80, 50);

}

void QOgreWindow::render()
{
    Ogre::WindowEventUtilities::messagePump();
    oRoot->renderOneFrame();
    //oRoot->startRendering();
    /*while(true)
    {
        Ogre::WindowEventUtilities::messagePump();
        if (oWin->isClosed()) break;
        if (!oRoot->renderOneFrame()) break;
    }*/

}

void QOgreWindow::renderLater()
{
    if (!isUpdatePending)
    {
        isUpdatePending = true;
        QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool QOgreWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
            isUpdatePending = false;
            renderNow();
            return true;
    default:
        return QWindow::event(event);
    }
}

void QOgreWindow::exposeEvent(QExposeEvent *e)
{
    Q_UNUSED(e);

    if (isExposed()) renderNow();
}

void QOgreWindow::renderNow()
{
    if (!isExposed()) return;
    if (oRoot == NULL) initialize();
    render();
    if (isAnimating) renderLater();
}

bool QOgreWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == this)
    {
        if (event->type() == QEvent::Resize)
        {
            if (isExposed() && oWin != NULL) oWin->resize(this->width(), this->height());
        }
    }
    return false;
}

// Keyboard handlers
void QOgreWindow::keyPressEvent(QKeyEvent *kev)
{
    if (camMan) camMan->injectKeyDown(*kev);
}

void QOgreWindow::keyReleaseEvent(QKeyEvent *kev)
{
    if (camMan) camMan->injectKeyUp(*kev);
}

void QOgreWindow::mouseMoveEvent(QMouseEvent *e)
{
    static int lastX = e->x();
    static int lastY = e->y();
    int relX = e->x() - lastX;
    int relY = e->y() - lastY;
    lastX = e->x();
    lastY = e->y();

    if (camMan && (e->buttons() & Qt::LeftButton))
        camMan->injectMouseMove(relX, relY);
}

void QOgreWindow::wheelEvent(QWheelEvent *e)
{
    if (camMan) camMan->injectWheelMove(*e);
}

void QOgreWindow::mousePressEvent(QMouseEvent *e)
{
    if (camMan) camMan->injectMouseDown(*e);
}

void QOgreWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if (camMan) camMan->injectMouseUp(*e);

    QPoint pos = e->pos();
    Ogre::Ray mouseRay = oCam->getCameraToViewportRay(
                (Ogre::Real)pos.x() / oWin->getWidth(),
                (Ogre::Real)pos.y() / oWin->getHeight());
    Ogre::RaySceneQuery* sceneQuery = oSceneMgr->createRayQuery(mouseRay);
    sceneQuery->setSortByDistance(true);
    Ogre::RaySceneQueryResult vResult = sceneQuery->execute();

    for (size_t ui = 0; ui < vResult.size(); ui++)
    {
        if (vResult[ui].movable)
        {
            if (vResult[ui].movable->getMovableType().compare("Entity") == 0)
            {
                emit entitySelected((Ogre::Entity*)vResult[ui].movable);
            }
        }
    }
    oSceneMgr->destroyQuery(sceneQuery);
}

void QOgreWindow::setAnimating(bool _isAnimating)
{
    isAnimating = _isAnimating;
    if (_isAnimating) renderLater();
}

bool QOgreWindow::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    if (oWin->isClosed()) return false;

    camMan->frameRenderingQueued(evt);
    return true;
}

void QOgreWindow::log(Ogre::String msg)
{
    if (Ogre::LogManager::getSingletonPtr() != NULL)
        Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}

void QOgreWindow::log(QString msg)
{
    log(Ogre::String(msg.toStdString().c_str()));
}
