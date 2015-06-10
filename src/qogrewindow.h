#ifndef QOGREWINDOW_H
#define QOGREWINDOW_H

// Qt Includes
#include <QtWidgets/QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QWindow>
#include <QMouseEvent>
#include <QTextStream>

// Ogre3D header
#include <Ogre.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>

// SDL
// For some reason the include in the .pro file didn't work, oh well
//#include <../SDL2-2.0.3/include/SDL.h>

// Qt-tified Camera Man Include
#include "qtogresdkcameraman.h"

// Include the Console window for debug purposes
#include <iostream>

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

    void setAnimating(bool _isAnimating);

    bool go();

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
    Ogre::String resConfig;
    Ogre::String pluginConfig;

    OgreQtBites::QtOgreSdkCameraMan* camMan;

    bool isUpdatePending;
    bool isAnimating;

    // Let's Use Qt's Keyboard and Mouse Handlers temporarily
    // If I then find out they work correctly, we dump SDL2 out of this project.
    virtual void keyPressEvent(QKeyEvent *kev);
    virtual void keyReleaseEvent(QKeyEvent *kev);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void exposeEvent(QExposeEvent *e);
    virtual bool event(QEvent *event);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);

    // Log Ogre stuff
    void log(Ogre::String msg);
    void log(QString msg);

};

#endif // QOGREWINDOW_H
