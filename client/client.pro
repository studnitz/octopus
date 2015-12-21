QT += core
QT += network
QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/client.cpp \
    src/gst_recorder.cpp

HEADERS += \
    src/client.h \
    src/gst_recorder.h
