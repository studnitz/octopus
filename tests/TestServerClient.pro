QT       += network testlib

QT       -= gui

TARGET = tst_communication
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_communication.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"


INCLUDEPATH += ../client/
INCLUDEPATH += ../server/

SOURCES += ../client/client.cpp
SOURCES += ../server/myserver.cpp
SOURCES += ../server/mythread.cpp


HEADERS += ../client/client.h
HEADERS += ../server/myserver.h
HEADERS += ../server/mythread.h
