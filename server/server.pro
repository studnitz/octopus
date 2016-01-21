QT += core
QT += network
QT -= gui

TARGET = server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
    src/server.cpp \
    src/serverthread.cpp \
    src/grid.cpp \
    src/recording.cpp \
    src/videofile.cpp

HEADERS += \
    src/server.h \
    src/serverthread.h \
    src/grid.h \
    src/recording.h \
    src/videofile.h


