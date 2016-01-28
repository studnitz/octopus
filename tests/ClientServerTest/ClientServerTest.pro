#-------------------------------------------------
#
# Project created by QtCreator 2016-01-18T15:01:11
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_communication
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_communication.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += \
    ../../client/src/ \
    ../../server/src/

SOURCES += \
    ../../client/src/client.cpp \
    ../../server/src/server.cpp \
    ../../server/src/serverthread.cpp



HEADERS += \
    ../../client/src/client.h \
    ../../server/src/server.h \
    ../../server/src/serverthread.h

