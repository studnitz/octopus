INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/src/server.cpp \
    $$PWD/src/serverthread.cpp \
    $$PWD/src/grid.cpp \
    $$PWD/src/recording.cpp \
    $$PWD/src/videofile.cpp \
    $$PWD/src/serverinterface.cpp \
    $$PWD/src/ftpdownloader.cpp \
    $$PWD/src/gst_exporter.cpp

HEADERS += \
    $$PWD/src/server.h \
    $$PWD/src/serverthread.h \
    $$PWD/src/grid.h \
    $$PWD/src/recording.h \
    $$PWD/src/videofile.h \
    $$PWD/src/serverinterface.h \
    $$PWD/src/ftpdownloader.h \
    $$PWD/src/gst_exporter.h

CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 \
    Qt5GLib-2.0
CONFIG   += c++11
