#-------------------------------------------------
#
# Project created by QtCreator 2015-11-30T12:18:17
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT-Server_wGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myserver.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    myserver.h \
    mythread.h

FORMS    += mainwindow.ui

CONFIG   += c++11
