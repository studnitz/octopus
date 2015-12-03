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


INCLUDEPATH += ../Qt-Client/
INCLUDEPATH += ../Qt-Server/

SOURCES += ../Qt-Client/client.cpp
SOURCES += ../Qt-Server/myserver.cpp
SOURCES += ../Qt-Server/mythread.cpp


HEADERS += ../Qt-Client/client.h
HEADERS += ../Qt-Server/myserver.h
HEADERS += ../Qt-Server/mythread.h

