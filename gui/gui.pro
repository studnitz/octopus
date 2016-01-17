    QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =    octopus-gui
TEMPLATE =  app


SOURCES += src/main.cpp         \
        src/mainwindow.cpp      \
        src/playlistmodel.cpp   \
        src/videoplayer.cpp     \
    ../server/src/server.cpp  \
    ../server/src/serverthread.cpp
HEADERS  += src/mainwindow.h    \
        src/playlistmodel.h     \
        src/videoplayer.h       \
    ../server/src/server.h    \
    ../server/src/serverthread.h

FORMS    += forms/mainwindow.ui

CONFIG   += c++11
