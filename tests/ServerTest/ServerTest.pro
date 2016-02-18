QT       += network testlib

QT       -= gui

TARGET = tst_serverbefehle
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_serverbefehle.cpp

include($$PWD/../../server/server.pri)
