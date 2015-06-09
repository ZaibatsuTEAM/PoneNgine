# PoneNgine

## About
PoneNgine is The Wapa Castle Team's take on making a Game Engine uses libraries found around the internet and putting them together. Also we want this engine to be the first 3D engine to ever support Qt, since there are none as far as research went.

## Libraries Used

* Rendering:      Ogre3D 1.9
* Physics:        PhysX 3.3
* Input:          Currentlty Qt's (QEvent, QKeyEvent, QMouseEvent)
* Audio:          Currently Qt's
* Networking:     ENet 1.3.13
* Misc.           Qt

## Compiling

* You need QtCreator with Qt 5.4.1 or higher with MSVC11 (Visual Studio 2012) as the compiler (If you can get this to compile with MinGW, I'll love you forever (no homo)).
* Have installed the Ogre 1.9 SDK, change the .pro file to suit your directories.
* Have the PhysX 3.3 SDK, change the .pro file to suit your directories.
* Have Enet included in the .pro file along with the necessary dependant libraries (e.g. WinShock2 for windows)
* If you have done all of these, you should be able to compile this. You may need to asign some .dll files to your executable directory.

## To-Do List

- [x] Get QOgreWindow compiled and running
- [ ] Get an Ogre mesh rendered in the QOgreWindow
- [ ] Get an entire Ogre scene rendered in the QOgreWindow
- [ ] Implement Physics with PhysX
- [ ] Implement Audio
- [x] Implement Networking

More Tasks may be added as development goes further.
