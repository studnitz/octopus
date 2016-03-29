INCLUDEPATH += $$PWD

SOURCES += $$PWD/src/mainwindow.cpp         \
        $$PWD/src/videoplayer.cpp           \
        $$PWD/src/playersettingsdialog.cpp  \
        $$PWD/src/settingsdialog.cpp        \
        $$PWD/src/playbackview.cpp          \
        $$PWD/src/recordingview.cpp \
    $$PWD/src/guiinterface.cpp \
    $$PWD/src/exportierendialog.cpp \
    $$PWD/src/clientgui.cpp \
    $$PWD/src/recordinggrid.cpp \
    $$PWD/src/recordinggridelement.cpp

HEADERS  += $$PWD/src/mainwindow.h          \
        $$PWD/src/videoplayer.h             \
        $$PWD/src/playersettingsdialog.h    \
        $$PWD/src/settingsdialog.h          \
        $$PWD/src/playbackview.h            \
        $$PWD/src/recordingview.h \
    $$PWD/src/guiinterface.h \
    $$PWD/src/exportierendialog.h \
    $$PWD/src/clientgui.h \
    $$PWD/src/recordinggrid.h \
    $$PWD/src/recordinggridelement.h

FORMS    += $$PWD/forms/mainwindow.ui \
    $$PWD/forms/settingsdialog.ui 	\
    $$PWD/forms/exportierendialog.ui 	\
    $$PWD/forms/playersettingsdialog.ui

include($$PWD/../server/server.pri)
