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
    sammedia.cpp \
    About/about.cpp \
    Settings/settings.cpp \
    Settings/boolsetting.cpp \
    Settings/divsetting.cpp \
    Settings/textsetting.cpp \
    Window/window.cpp \
    Window/serverinfo.cpp \
    Window/scrolltext.cpp \
    Window/songcaster.cpp \
    Window/timebar.cpp \
    Window/toolbar.cpp \
    Window/SongDisplay/songdisplay.cpp \
    Window/SongDisplay/albumart.cpp \
    Window/SongDisplay/songinfo.cpp \
    History/history.cpp \
    History/memory.cpp \
    History/song.cpp

HEADERS += sammedia.h \
    About/about.h \
    Settings/settings.h \
    Settings/boolsetting.h \
    Settings/divsetting.h \
    Settings/textsetting.h \
    Window/window.h \
    Window/serverinfo.h \
    Window/scrolltext.h \
    Window/songcaster.h \
    Window/timebar.h \
    Window/toolbar.h \
    Window/SongDisplay/songdisplay.h \
    Window/SongDisplay/albumart.h \
    Window/SongDisplay/songinfo.h \
    History/history.h \
    History/memory.h \
    History/song.h

OTHER_FILES += \
    android/AndroidManifest.xml \
    LICENSE \
    README.md
