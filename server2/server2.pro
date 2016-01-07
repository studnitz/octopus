QT += core
QT += network
QT -= gui

TARGET = server2
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    server.cpp \
    clientconnection.cpp

HEADERS += \
    server.h \
    clientconnection.h

