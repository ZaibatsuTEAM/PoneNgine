# PoneNgine

## About
PoneNgine is The Wapa Castle Team's take on making a Game Engine uses libraries found around the internet and putting them together. Also we want this engine to be the first 3D engine to ever support Qt, since there are none as far as research went.

## Libraries Used

* Rendering:      Ogre3D 1.9
* Physics:        PhysX 3.3
* Input:          SDL2 (Because Qt's sucks for games)
* Audio:          Currently Qt's for testing
* Networking:     None, yet.
* Misc.           Qt

## Compiling

* You need QtCreator with Qt 5.4.1 or higher with MSVC11 (Visual Studio 2012) as the compiler (If you can get this to compile with MinGW, I'll love you forever (no homo)).
* Have installed the Ogre 1.9 SDK, change the .pro file to suit your directories.
* Have the PhysX 3.3 SDK, change the .pro file to suit your directories.
* If you have done all of these, you should be able to compile this. You may need to asign some .dll files to your executable directory.
