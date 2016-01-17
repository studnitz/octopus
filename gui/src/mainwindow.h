#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../server/src/server.h"
#include "../server/src/serverthread.h"
#include <QHostInfo>

// Videoplayer
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "playlistmodel.h"
#include "videoplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Server* server;

private slots:
    void on_recordButton_clicked();

    void on_pushButton_2_clicked();

    void on_playButton_clicked();

    void on_openFileButton_clicked();

    void on_stopButton_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

    void videoPlayerClicked(int index);

    void on_addPlayerButton_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> *clients;    // TODO: Kann weg?

    QList<QMediaPlayer*> *player;
    QList<VideoPlayer*> *videoPlayer;
    PlaylistModel *playlistModel;
    QMediaPlaylist *playlist;


    void recordStart();
    void recordStop();
    void printClients();
    void log(QString msg);
};

#endif // MAINWINDOW_H
