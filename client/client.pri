INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/src/client.cpp \
    $$PWD/src/gst_recorder.cpp

HEADERS += \
    $$PWD/src/client.h \
    $$PWD/src/gst_recorder.h

CONFIG += link_pkgconfig
PKGCONFIG += Qt5GStreamer-1.0 \
    Qt5GLib-2.0
CONFIG   += c++11
