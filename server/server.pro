QT += core
QT += network
QT -= gui

TARGET = server2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/server.cpp \
    src/serverthread.cpp

HEADERS += \
    src/server.h \
    src/serverthread.h

