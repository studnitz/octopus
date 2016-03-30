QT       += network testlib
QT       -= gui

TARGET = tst_communication
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_communication.cpp

include($$PWD/../../server/server.pri)
include($$PWD/../../client/client.pri)

CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 \
    Qt5GLib-2.0

CONFIG   += c++11

QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0

LIBS += \
    -lgcov
