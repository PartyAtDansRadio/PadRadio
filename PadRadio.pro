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
    Radio/radio.cpp \
    Radio/serverinfo.cpp \
    Radio/scrolltext.cpp \
    Radio/songcaster.cpp \
    Radio/timebar.cpp \
    Radio/toolbar.cpp \
    Radio/SongDisplay/songdisplay.cpp \
    Radio/SongDisplay/albumart.cpp \
    Radio/SongDisplay/songinfo.cpp \
    History/history.cpp \
    History/memory.cpp \
    History/song.cpp \
    window.cpp

HEADERS += sammedia.h \
    About/about.h \
    Settings/settings.h \
    Settings/boolsetting.h \
    Settings/divsetting.h \
    Settings/textsetting.h \
    Radio/radio.h \
    Radio/serverinfo.h \
    Radio/scrolltext.h \
    Radio/songcaster.h \
    Radio/timebar.h \
    Radio/toolbar.h \
    Radio/SongDisplay/songdisplay.h \
    Radio/SongDisplay/albumart.h \
    Radio/SongDisplay/songinfo.h \
    History/history.h \
    History/memory.h \
    History/song.h \
    window.h

OTHER_FILES += \
    android/AndroidManifest.xml \
    LICENSE \
    README.md
