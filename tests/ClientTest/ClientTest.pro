QT       += network testlib
QT       -= gui

TARGET = tst_clientbefehle
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_clientbefehle.cpp

include($$PWD/../../client/client.pri)

CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 \
    Qt5GLib-2.0
