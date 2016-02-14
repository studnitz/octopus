#ifndef PLAYBACKVIEW_H
#define PLAYBACKVIEW_H

#include "mainwindow.h"

class PlaybackView
{
public:
    PlaybackView(MainWindow* mW, QWidget* t);
    MainWindow* mainWindow;
    QWidget* tab;
};

#endif // PLAYBACKVIEW_H
