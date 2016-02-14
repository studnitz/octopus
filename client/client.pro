QT += core
QT += network
QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp

include(client.pri)

CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 \
    Qt5GLib-2.0
