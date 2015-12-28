#-------------------------------------------------
#
# Project created by QtCreator 2015-12-03T17:04:22
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
    ../client/src/ \
    ../server/src/

SOURCES += \
    ../client/src/client.cpp \
    ../server/src/myserver.cpp \
    ../server/src/mythread.cpp


HEADERS += \
    ../client/src/client.h \
    ../server/src/myserver.h \
    ../server/src/mythread.h
