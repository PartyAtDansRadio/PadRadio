#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T15:29:27
#
#-------------------------------------------------

QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PadRadio

TEMPLATE = app

SOURCES += main.cpp\
    Window/window.cpp \
    Window/scrolltext.cpp \
    Window/sammedia.cpp \
    About/about.cpp \
    Settings/settings.cpp \
    Settings/boolsetting.cpp \
    Settings/textsetting.cpp \
    Settings/divsetting.cpp

HEADERS += Window/window.h \
    Window/scrolltext.h \
    Window/sammedia.h \
    About/about.h \
    Settings/settings.h \
    Settings/boolsetting.h \
    Settings/textsetting.h \
    Settings/divsetting.h

FORMS += Window/window.ui

CONFIG += mobility c++11

MOBILITY += messaging

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    LICENSE \
    README.md

RESOURCES += Files.qrc
