QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = octopus-gui
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    ../server/src/myserver.cpp \
    ../server/src/mythread.cpp

HEADERS  += src/mainwindow.h \
    ../server/src/myserver.h \
    ../server/src/mythread.h

FORMS    += forms/mainwindow.ui

CONFIG   += c++11