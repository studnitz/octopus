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
    bool showPercentage = 0;

signals:
    /**
     * @brief getinfo Signals Server to broadcast commands to get info.
     */
    void getinfo();

public slots:
    /**
     * @brief on_recordButton_clicked GUI
     */
    void on_recordButton_clicked();

    /**
     * @brief on_pushButton_2_clicked GUI
     */
    void on_pushButton_2_clicked();

    /**
     * @brief on_playButton_clicked GUI
     */
    void on_playButton_clicked();

    /**
     * @brief continueUpdateClientList updates the Client List in the GUI
     */
    void continueUpdateClientList();

    /**
     * @brief on_openFileButton_clicked GUI
     */
    void on_openFileButton_clicked();

    /**
     * @brief on_stopButton_clicked GUI
     */
    void on_stopButton_clicked();

    /**
     * @brief on_listView_doubleClicked GUI
     * @param index
     */
    void on_listView_doubleClicked(const QModelIndex &index);

    void videoPlayerClicked(int index);
    void on_addPlayerButton_clicked();

private slots:
    /**
     * @brief on_pushButton_Percent_clicked GUI toggle Percent in LEDs
     */
    void on_pushButton_Percent_clicked();

private:
    Ui::MainWindow *ui;
    QList<QString> *clients;    // TODO: Kann weg?

    QList<QMediaPlayer*> *player;
    QList<VideoPlayer*> *videoPlayer;
    PlaylistModel *playlistModel;
    QMediaPlaylist *playlist;


    void recordStart();
    void recordStop();
    /**
     * @brief printClients
     */
    void printClients();
    /**
     * @brief log logs into GUI-log
     * @param msg
     */
    void log(QString msg);
    /**
     * @brief getColorFromPercent Gibt einen Farbwert zwischen gruen(0%) und rot(100%) zurueck aufgrund von Prozentzahlen.
     * @param percent int zwischen 0 und 100
     * @return QColor zwischen gruen und rot
     */
    QColor getColorFromPercent(int percent);
};

#endif // MAINWINDOW_H
