#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T15:29:27
#
#-------------------------------------------------

QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PadRadio

TEMPLATE = app

INCLUDEPATH += $$PWD

SOURCES += main.cpp\
    window.cpp \
    $$PWD/scrolltext.cpp \
    sammedia.cpp \
    about.cpp \
    settings.cpp

HEADERS += window.h \
    $$PWD/scrolltext.h \
    sammedia.h \
    about.h \
    settings.h

FORMS   += window.ui \
    about.ui \
    settings.ui

CONFIG  += mobility c++11

MOBILITY += messaging

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    LICENSE \
    README.md

RESOURCES += Files.qrc
