QT += core
QT += network
QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/myserver.cpp \
    src/mythread.cpp

HEADERS += \
    src/myserver.h \
    src/mythread.h

