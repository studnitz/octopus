INCLUDEPATH += $$PWD

SOURCES += $$PWD/src/mainwindow.cpp   \
        $$PWD/src/playlistmodel.cpp   \
        $$PWD/src/videoplayer.cpp     \
        $$PWD/src/playersettingsdialog.cpp \
        $$PWD/src/settingsdialog.cpp

HEADERS  += $$PWD/src/mainwindow.h    \
        $$PWD/src/playlistmodel.h     \
        $$PWD/src/videoplayer.h       \
        $$PWD/src/playersettingsdialog.h \
        $$PWD/src/settingsdialog.h

FORMS    += $$PWD/forms/mainwindow.ui \
    $$PWD/forms/settingsdialog.ui 	\
    $$PWD/forms/playersettingsdialog.ui

include($$PWD/../server/server.pri)
