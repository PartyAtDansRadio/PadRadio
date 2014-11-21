#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T15:29:27
#
#-------------------------------------------------

QT += core gui widgets multimedia
TARGET = PadRadio
TEMPLATE = app
CONFIG += mobility c++11
MOBILITY += messaging
RESOURCES += Files.qrc
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

SOURCES += main.cpp\
    Window/window.cpp \
    Window/scrolltext.cpp \
    Window/sammedia.cpp \
    About/about.cpp \
    Settings/settings.cpp \
    Settings/boolsetting.cpp \
    Settings/textsetting.cpp \
    Settings/divsetting.cpp \
    Window/timebar.cpp \
    Window/toolbar.cpp \
    Window/serverinfo.cpp \
    Window/songcaster.cpp \
    Window/SongDisplay/albumart.cpp \
    Window/SongDisplay/songinfo.cpp \
    Window/SongDisplay/songdisplay.cpp

HEADERS += Window/window.h \
    Window/scrolltext.h \
    Window/sammedia.h \
    About/about.h \
    Settings/settings.h \
    Settings/boolsetting.h \
    Settings/textsetting.h \
    Settings/divsetting.h \
    Window/timebar.h \
    Window/toolbar.h \
    Window/serverinfo.h \
    Window/songcaster.h \
    Window/SongDisplay/albumart.h \
    Window/SongDisplay/songinfo.h \
    Window/SongDisplay/songdisplay.h

OTHER_FILES += \
    android/AndroidManifest.xml \
    LICENSE \
    README.md

FORMS += Window/window.ui
