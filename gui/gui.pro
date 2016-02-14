QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =    octopus-gui
TEMPLATE =  app

SOURCES += src/main.cpp

include(gui.pri)

CONFIG   += c++11
