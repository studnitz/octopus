QT       += network testlib
QT       -= gui

TARGET = tst_clientbefehle
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_clientbefehle.cpp

include($$PWD/../../client/client.pri)

CONFIG   += c++11

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov
