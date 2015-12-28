QT += core
QT += network
QT -= gui

TARGET = client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/client.cpp \
    src/main.cpp

HEADERS += \
    src/client.h
