    QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = octopus-gui
TEMPLATE = app


SOURCES += src/main.cpp         \
        src/mainwindow.cpp      \
        src/playlistmodel.cpp   \
    ../server/server.cpp        \
    ../server/serverthread.cpp

HEADERS  += src/mainwindow.h    \
    src/playlistmodel.h         \
    ../server/server.h          \
    ../server/serverthread.h

FORMS    += forms/mainwindow.ui

CONFIG   += c++11
