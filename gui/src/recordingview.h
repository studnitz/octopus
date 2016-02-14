#ifndef RECORDINGVIEW_H
#define RECORDINGVIEW_H

#include "mainwindow.h"


class RecordingView
{
public:
    MainWindow* mainWindow;
    QWidget* tab;
    RecordingView(MainWindow* mW, QWidget* t);
};

#endif // RECORDINGVIEW_H
