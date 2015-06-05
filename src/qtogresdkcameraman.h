/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/

 Copyright (c) 2000-2014 Torus Knot Software Ltd

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

/*
 -----------------------------------------------------------------------------
 This is a mofied version of the Ogre SDK's SdkCameraMan.h file for the use of
 the Qt Library instead of OIS. Taken from the Wiki.

 Used for PoneNgine.

 Yes I litteraly typed this entire file, I just feel like typing code is
 better than copying and pasting. I learn some bits in the process too!
 -----------------------------------------------------------------------------
*/

#ifndef QTOGRESDKCAMERAMAN_H
#define QTOGRESDKCAMERAMAN_H

#include <Ogre.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreFrameListener.h>
#include <QKeyEvent>
#include <QMouseEvent>

#include <limits>

//#include <../SDL2-2.0.3/include/SDL.h>

namespace OgreQtBites
{
    enum CameraStyle
    {
        CS_FREELOOK,
        CS_ORBIT,
        CS_MANUAL
    };
    /*=============================================================================
        | Utility class for controlling the camera in samples.
    =============================================================================*/
    class QtOgreSdkCameraMan
    {
    public:
        QtOgreSdkCameraMan(Ogre::Camera* cam)
            : oCam(0)
            , oTarget(0)
            , oOrbiting(false)
            , oZooming(false)
            , oTopSpeed(150)
            , oVelocity(Ogre::Vector3::ZERO)
            , oGoingForward(false)
            , oGoingBack(false)
            , oGoingLeft(false)
            , oGoingRight(false)
            , oGoingUp(false)
            , oGoingDown(false)
            , oFastMove(false)
        {
            setCamera(cam);
            setStyle(CS_FREELOOK);
        }

        virtual ~QtOgreSdkCameraMan() {}

        /*-----------------------------------------------------------------------------
                | Swaps the camera on our camera man for another camera.
        -----------------------------------------------------------------------------*/

        virtual void setCamera(Ogre::Camera* cam) { oCam = cam; }
        virtual Ogre::Camera* getCamera() { return oCam; }

        /*-----------------------------------------------------------------------------
                | Sets the target we will revolve around. Only applies for orbit style.
        -----------------------------------------------------------------------------*/

        virtual void setTarget(Ogre::SceneNode* target)
        {
            if (target != oTarget)
            {
                oTarget = target;
                if (target)
                {
                    setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
                    oCam->setAutoTracking(true, oTarget);
                }
                else
                {
                    oCam->setAutoTracking(false);
                }
            }
        }

        virtual Ogre::SceneNode* getTarget() { return oTarget; }

        /*-----------------------------------------------------------------------------
                | Sets the spatial offset from the target. Only applies for orbit style.
        -----------------------------------------------------------------------------*/

        virtual void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist)
        {
            oCam->setPosition(oTarget->_getDerivedPosition());
            oCam->setOrientation(oTarget->_getDerivedOrientation());
            oCam->yaw(yaw);
            oCam->pitch(pitch);
            oCam->moveRelative(Ogre::Vector3(0,0,dist));
        }

        /*-----------------------------------------------------------------------------
                | Sets the camera's top speed. Only applies for free-look style.
        -----------------------------------------------------------------------------*/

        virtual void setTopSpeed(Ogre::Real topSpeed) { oTopSpeed = topSpeed; }
        virtual Ogre::Real getTopSpeed() { return oTopSpeed; }

        /*-----------------------------------------------------------------------------
                | Sets the movement style of our camera man.
        -----------------------------------------------------------------------------*/

        virtual void setStyle(CameraStyle style)
        {
            if (oStyle != CS_ORBIT && style == CS_ORBIT)
            {
                setTarget(oTarget ? oTarget : oCam->getSceneManager()->getRootSceneNode());
                oCam->setFixedYawAxis(true);
                manualStop();
                setYawPitchDist(Ogre::Degree(0), Ogre::Degree(15), 150);
            }
            else if (oStyle != CS_FREELOOK && style == CS_FREELOOK)
            {
                oCam->setAutoTracking(false);
                oCam->setFixedYawAxis(true);
            }
            else if (oStyle != CS_MANUAL && style == CS_MANUAL)
            {
                oCam->setAutoTracking(false);
                manualStop();
            }
            oStyle = style;
        }

        virtual CameraStyle getStyle() { return oStyle; }

        /*-----------------------------------------------------------------------------
                | Manually stops the camera when in free-look mode.
        -----------------------------------------------------------------------------*/

        virtual void manualStop()
        {
            if (oStyle == CS_FREELOOK)
            {
                oGoingForward   = false;
                oGoingBack      = false;
                oGoingLeft      = false;
                oGoingRight     = false;
                oGoingUp        = false;
                oGoingDown      = false;
                oVelocity       = Ogre::Vector3::ZERO;
            }
        }

        virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt)
        {
            if (oStyle == CS_FREELOOK)
            {
                // build our acceleration vector based on keyboard input composite
                Ogre::Vector3 accel = Ogre::Vector3::ZERO;
                if (oGoingForward)  accel += oCam->getDirection();
                if (oGoingBack)     accel -= oCam->getDirection();
                if (oGoingRight)    accel += oCam->getRight();
                if (oGoingLeft)     accel -= oCam->getRight();
                if (oGoingUp)       accel += oCam->getUp();
                if (oGoingDown)     accel -= oCam->getUp();

                // if accelerating, try to reach top speed in a certain time
                Ogre::Real topSpeed = oFastMove ? oTopSpeed * 20 : oTopSpeed;
                if (accel.squaredLength() != 0)
                {
                    accel.normalise();
                    oVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
                }
                // if not accelerating, try to stop in a certain time
                else oVelocity -= oVelocity * evt.timeSinceLastFrame * 10;

                Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

                // keep camera velocity below top speed and above epsilon
                if (oVelocity.squaredLength() > topSpeed * topSpeed)
                {
                    oVelocity.normalise();
                    oVelocity *= topSpeed;
                }
                else if (oVelocity.squaredLength() < tooSmall * tooSmall)
                {
                    oVelocity = Ogre::Vector3::ZERO;
                }

                if (oVelocity != Ogre::Vector3::ZERO) oCam->move(oVelocity * evt.timeSinceLastFrame);
            }
            return true;
        }

        /*-----------------------------------------------------------------------------
                | Processes key presses for free-look style movement.
        -----------------------------------------------------------------------------*/

        // Qt's Keyboard handler
        virtual void injectKeyDown(const QKeyEvent& evt)
        {
            if (oStyle == CS_FREELOOK)
            {
                if (evt.key() == Qt::Key_W || evt.key() == Qt::Key_Up) oGoingForward = true;
                else if (evt.key() == Qt::Key_S || evt.key() == Qt::Key_Down) oGoingBack = true;
                else if (evt.key() == Qt::Key_A || evt.key() == Qt::Key_Left) oGoingLeft = true;
                else if (evt.key() == Qt::Key_D || evt.key() == Qt::Key_Right) oGoingRight = true;
                else if (evt.key() == Qt::Key_PageUp) oGoingUp = true;
                else if (evt.key() == Qt::Key_PageDown) oGoingDown = true;
                else if (evt.key() == Qt::Key_Shift) oFastMove = true;
            }
        }

        // SDL2's Keyboard Handler
        /*virtual void injectKeyDown(const SDL_KeyboardEvent &evt)
        {
            if (evt.keysym == SDL_SCANCODE_W || evt.keysym == SDL_SCANCODE_UP) oGoingForward = true;
            else if (evt.keysym == SDL_SCANCODE_S || evt.keysym == SDL_SCANCODE_DOWN) oGoingBack = true;
            else if (evt.keysym == SDL_SCANCODE_A || evt.keysym == SDL_SCANCODE_LEFT) oGoingLeft = true;
            else if (evt.keysym == SDL_SCANCODE_D || evt.keysym == SDL_SCANCODE_RIGHT) oGoingRight = true;
            else if (evt.keysym == SDL_SCANCODE_PAGEUP) oGoingUp = true;
            else if (evt.keysym == SDL_SCANCODE_PAGEDOWN) oGoingDown = true;
            else if (evt.keysym == SDL_SCANCODE_LSHIFT) oFastMove = true;
        }*/

        /*-----------------------------------------------------------------------------
                | Processes key releases for free-look style movement.
        -----------------------------------------------------------------------------*/

        // Qt's Keyboard Handler
        virtual void injectKeyUp(const QKeyEvent& evt)
        {
            if (oStyle == CS_FREELOOK)
            {
                if (evt.key() == Qt::Key_W || evt.key() == Qt::Key_Up) oGoingForward = false;
                else if (evt.key() == Qt::Key_S || evt.key() == Qt::Key_Down) oGoingBack = false;
                else if (evt.key() == Qt::Key_A || evt.key() == Qt::Key_Left) oGoingLeft = false;
                else if (evt.key() == Qt::Key_D || evt.key() == Qt::Key_Right) oGoingRight = false;
                else if (evt.key() == Qt::Key_PageUp) oGoingUp = false;
                else if (evt.key() == Qt::Key_PageDown) oGoingDown = false;
                else if (evt.key() == Qt::Key_Shift) oFastMove = false;
            }
        }

        // SDL2's Keyboard Handler
        /*virtual void injectKeyUp(const SDL_KeyboardEvent &evt)
        {
            if (evt.keysym == SDL_SCANCODE_W || evt.keysym == SDL_SCANCODE_UP) oGoingForward = false;
            else if (evt.keysym == SDL_SCANCODE_S || evt.keysym == SDL_SCANCODE_DOWN) oGoingBack = false;
            else if (evt.keysym == SDL_SCANCODE_A || evt.keysym == SDL_SCANCODE_LEFT) oGoingLeft = false;
            else if (evt.keysym == SDL_SCANCODE_D || evt.keysym == SDL_SCANCODE_RIGHT) oGoingRight = false;
            else if (evt.keysym == SDL_SCANCODE_PAGEUP) oGoingUp = false;
            else if (evt.keysym == SDL_SCANCODE_PAGEDOWN) oGoingDown = false;
            else if (evt.keysym == SDL_SCANCODE_LSHIFT) oFastMove = false;
        }*/

        /*-----------------------------------------------------------------------------
                | Processes mouse movement differently for each style.
        -----------------------------------------------------------------------------*/

        virtual void injectMouseMove(int relX, int relY)
        {
            //            static int lastX = evt.x();
            //            static int lastY = evt.y();
            //            int relX = evt.x() - lastX;
            //            int relY = evt.y() - lastY;
            //            lastX = evt.x();
            //            lastY = evt.y();
            if (oStyle == CS_ORBIT)
            {
                Ogre::Real dist = (oCam->getPosition() - oTarget->_getDerivedPosition()).length();

                if (oOrbiting) // yaw around the target and pitch locally
                {
                    oCam->setPosition(oTarget->_getDerivedPosition());
                    oCam->yaw(Ogre::Degree(-relX * 0.025f));
                    oCam->pitch(Ogre::Degree(-relY * 0.025f));
                    oCam->moveRelative(Ogre::Vector3(0,0,dist));

                    // don't let the camera go over the top or around the bottom of the target
                }
                else if (oZooming) // move the camera toward or away from the target
                {
                    // the further the camera is, the faster it moves
                    oCam->moveRelative(Ogre::Vector3(0,0, relY * 0.004f * dist));
                }
            }
            else if (oStyle == CS_FREELOOK)
            {
                oCam->yaw(Ogre::Degree(-relX * 0.15f));
                oCam->pitch(Ogre::Degree(-relY * 0.15f));
            }
        }

        /*-----------------------------------------------------------------------------
                | Processes mouse movement differently for each style.
        -----------------------------------------------------------------------------*/

        virtual void injectWheelMove(const QWheelEvent& evt)
        {
            int relZ = evt.delta();
            if (oStyle == CS_ORBIT)
            {
                Ogre::Real dist = (oCam->getPosition() - oTarget->_getDerivedPosition()).length();

                if (relZ != 0) // move the camera toward or away from the target
                {
                    // the further the camera is, the faster it moves
                    oCam->moveRelative(Ogre::Vector3(0,0, -relZ * 0.0008f * dist));
                }
            }
        }

        /*-----------------------------------------------------------------------------
                | Processes mouse presses. Only applies for orbit style.
                | Left button is for orbiting, and right button is for zooming.
        -----------------------------------------------------------------------------*/

        virtual void injectMouseDown(const QMouseEvent& evt)
        {
            if (oStyle == CS_ORBIT)
            {
                if (evt.buttons() & Qt::LeftButton) oOrbiting = true;
                else if (evt.buttons() & Qt::RightButton) oZooming = true;
            }
        }

        /*-----------------------------------------------------------------------------
                | Processes mouse releases. Only applies for orbit style.
                | Left button is for orbiting, and right button is for zooming.
        -----------------------------------------------------------------------------*/

        virtual void injectMouseUp(const QMouseEvent& evt)
        {
            if (oStyle == CS_ORBIT)
            {
                if (evt.buttons() & Qt::LeftButton) oOrbiting = false;
                else if (evt.buttons() & Qt::RightButton) oZooming = false;
            }
        }

    protected:
        Ogre::Camera* oCam;
        CameraStyle oStyle;
        Ogre::SceneNode* oTarget;
        bool oOrbiting;
        bool oZooming;
        Ogre::Real oTopSpeed;
        Ogre::Vector3 oVelocity;
        bool oGoingForward;
        bool oGoingBack;
        bool oGoingLeft;
        bool oGoingRight;
        bool oGoingUp;
        bool oGoingDown;
        bool oFastMove;
    };
}

#endif // QTOGRESDKCAMERAMAN_H
