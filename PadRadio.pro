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
    $$PWD/albumart.cpp \
    sammedia.cpp

HEADERS += window.h \
    $$PWD/scrolltext.h \
    $$PWD/albumart.h \
    sammedia.h

FORMS   += window.ui

CONFIG  += mobility

MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml \
    LICENSE \
    README.md

RESOURCES += Files.qrc
