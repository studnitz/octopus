#-------------------------------------------------
#
# Project created by QtCreator 2016-01-18T15:02:07
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_clientbefehle
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_clientbefehle.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += \
    ../../client/src/

SOURCES += \
    ../../client/src/client.cpp

HEADERS += \
    ../../client/src/client.h
