    QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =    octopus-gui
TEMPLATE =  app


SOURCES += src/main.cpp         \
        src/mainwindow.cpp      \
        src/playlistmodel.cpp   \
        src/videoplayer.cpp     \
    ../server/src/server.cpp  \
    ../server/src/serverthread.cpp \
    ../server/src/recording.cpp \
    ../server/src/grid.cpp       \
    ../server/src/videofile.cpp \
    src/playersettingsdialog.cpp \
    src/settingsdialog.cpp \
    src/recordingview.cpp \
    src/playbackview.cpp

HEADERS  += src/mainwindow.h    \
        src/playlistmodel.h     \
        src/videoplayer.h       \
    ../server/src/server.h    \
    ../server/src/serverthread.h \
    ../server/src/grid.h        \
    ../server/src/videofile.h \
    src/playersettingsdialog.h \
    src/settingsdialog.h \
    src/playbackview.h \
    src/recordingview.h

FORMS    += forms/mainwindow.ui \
    forms/settingsdialog.ui 	\
    forms/playersettingsdialog.ui

CONFIG   += c++11
