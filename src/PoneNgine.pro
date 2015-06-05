# PoneNgine Qt Project file

QT += core gui network opengl quick multimedia

TEMPLATE = app
TARGET = PoneEngine-Build-0001

CONFIG += c++11
CONFIG -= UNICODE

unix {
    # Standard Ogre3D Linux include
    INCLUDEPATH += /usr/include/OGRE
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
}

osx {
    # To be implemented
}

# Uses MSVC11 (Visual Studio 2012) Compiler
# Was going to use MinGW, but these libraries don't offer MinGW versions.
win32 {
    # Because Ogre needs this
    LIBS *= user32.lib

    # You will need to modify these directories for compiling

    # Ogre LIBS and includes
    debug:LIBS += -LE:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/lib/debug
    release:LIBS += -LE:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/lib/release
    LIBS += -LE:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/boost/lib

    # OIS is there for Ogre Practicing purposes
    # Were going to use SDL2 for input in the main thing
    # I am still deciding from Qt's, OpenAL, FMod and SDL's Audio Libraries, any recommendations?

    INCLUDEPATH += \
        E:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/include \
        E:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/include/OGRE \
        E:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/include/OIS \
        E:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/boost \
        E:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/Samples/Commong/include \
        E:/Software_Dev/PoneNgine/OgreSDK_vc11_v1-9-0/include/Ogre/Overlay

    # PhysX LIBS and includes
    LIBS += -LE:/Software_Dev/PoneNgine/PhysX/Lib/vc11win32
    INCLUDEPATH += E:/Software_Dev/PoneNgine/PhysX/Include

    # SDL2 Libs and includes
    LIBS += -LE:/Software_Dev/PoneNgine/SDL2-2.0.3/lib/x86
    INCLUDEPATH += -LE:/Software_Dev/PoneNgine/SDL2-2.0.3/include
}

debug {
    TARGET = $$join(TARGET,,,d)
    LIBS *= -lOgreMain_d -lOIS_d -lOgreTerrain_d -lSDL2 -lPhysX3_x86
}

release:LIBS *= -lOgreMain -lOIS -lOgreOverlay -lOgreTerrain -lSDL2 -lPhysX3_x86

SOURCES += \
    main.cpp \
    qogrewindow.cpp

HEADERS += \
    qogrewindow.h \
    qtogresdkcameraman.h

FORMS +=

