INCLUDEPATH += $$PWD

SOURCES += $$PWD/src/mainwindow.cpp         \
        $$PWD/src/playlistmodel.cpp         \
        $$PWD/src/videoplayer.cpp           \
        $$PWD/src/playersettingsdialog.cpp  \
        $$PWD/src/settingsdialog.cpp        \
        $$PWD/src/playbackview.cpp          \
        $$PWD/src/recordingview.cpp

HEADERS  += $$PWD/src/mainwindow.h          \
        $$PWD/src/playlistmodel.h           \
        $$PWD/src/videoplayer.h             \
        $$PWD/src/playersettingsdialog.h    \
        $$PWD/src/settingsdialog.h          \
        $$PWD/src/playbackview.h            \
        $$PWD/src/recordingview.h

FORMS    += $$PWD/forms/mainwindow.ui \
    $$PWD/forms/settingsdialog.ui 	\
    $$PWD/forms/playersettingsdialog.ui

include($$PWD/../server/server.pri)
