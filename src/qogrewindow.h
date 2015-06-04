#ifndef QOGREWINDOW_H
#define QOGREWINDOW_H

// Qt Includes
#include <QtWidgets/QApplication>
#include <QtGui/QKeyEvent>
#include <QtGui/QWindow>

// Ogre3D header
#include <Ogre.h>

// SDL
// For some reason the include in the .pro file didn't work, oh well
//#include <../SDL2-2.0.3/include/SDL.h>

// Qt-rified Camera Man Include
#include "qtogresdkcameraman.h"

class QOgreWindow : public QWindow, public Ogre::FrameListener
{
    Q_OBJECT
public:
    explicit QOgreWindow(QWindow *parent = NULL);
    ~QOgreWindow();

public:
    // These are virtual so we can allow further inheritance
    virtual void render(QPainter *painter);
    virtual void render();
    virtual void initialize();
    virtual void createScene();
#if OGRE_VERSION >= ((2 << 16) | (0 << 8) | 0)
    virtual void createCompositor();
#endif

    void setAnimating(bool isAnimating);

public slots:
    virtual void renderLater();
    virtual void renderNow();

    virtual bool eventFilter(QObject *target, QEvent *event);

signals:
    void entitySelected(Ogre::Entity* entity);

protected:
    Ogre::Root* oRoot;
    Ogre::RenderWindow* oWin;
    Ogre::SceneManager* oSceneMgr;
    Ogre::Camera* oCam;
    Ogre::ColourValue oBgColor;

    OgreQtBites::QtOgreSdkCameraMan* camMan;

    bool isUpdatePending;
    bool isAnimating;

    // Use SDL for keyboard, Qt for mouse (temporary until I learn more about SDL2's Mosue Handling)
    // I swear this is a bad idea using two input handlers at the same time.
    virtual void keyPressEvent(QKeyEvent * kev);
    virtual void keyReleaseEvent(QKeyEvent * kev);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void exposeEvent(QExposeEvent *e);
    virtual bool event(QEvent *event);

    // Log Ogre stuff
    void log(Ogre::String msg);
    void log(QString msg);

};

#endif // QOGREWINDOW_H
