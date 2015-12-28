#-------------------------------------------------
#
# Project created by QtCreator 2015-12-28T15:51:49
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = tst_clientbefehlteststest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += ../client/src/

SOURCES += tst_clientbefehlteststest.cpp
SOURCES += ../client/src/client.cpp
HEADERS += ../client/src/client.h
DEFINES += SRCDIR=\\\"$$PWD/\\\"
