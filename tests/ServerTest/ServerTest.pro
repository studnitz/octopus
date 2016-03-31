QT       += network testlib

QT       -= gui

TARGET = tst_serverbefehle
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_serverbefehle.cpp

include($$PWD/../../server/server.pri)

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov
