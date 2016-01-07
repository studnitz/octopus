QT += core
QT += network
QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/myserver.cpp \
    src/main.cpp

HEADERS += \
    src/myserver.h
