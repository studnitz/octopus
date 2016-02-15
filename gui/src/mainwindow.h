#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../server/src/server.h"
#include "../server/src/serverthread.h"
#include <QHostInfo>
#include <QSettings>
#include "settingsdialog.h"

// Videoplayer
#include <QMediaPlayer>
#include <QListWidget>
#include <QMediaPlaylist>
#include "playlistmodel.h"
#include "videoplayer.h"

#include "../server/src/recording.h"
#include "src/playersettingsdialog.h"

#include "playbackview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  Server *server;
  bool showPercentage = 0;
  QString versionOctopus = "0.313b";  // Versionnumber
  Ui::MainWindow *ui;

  /**
   * @brief settings to store settings
   * settings-file is in ~/.config/TUD\ IAD/octopus
   */
  QSettings *settings = new QSettings(QString("TUD IAD"), QString("octopus"));

  /**
   * @brief videoPlayer List of video players
   * The VideoPlayer class inherits from QVideoPlayer. It is the output object
   * for the stream that is decoded by the player object. Also it has some
   * overridden events compared to QVideoPlayer.
   */
  QList<VideoPlayer *> *videoPlayer;

  /**
   * @brief player List of players
   * The players implement the functionality. They hold references to the files
   * and it's also them who get calls for stopping or playing the sources.
   */
  QList<QMediaPlayer *> *player;

  void loadPlayersFromRecording();

  /**
   * @brief log Helper function for writing info messages into the log section.
   * @param msg QString that holds the message
   */
  void log(QString msg);

  Recording *recording;

  /**
   * @brief connectSourceToNewVideo Connects source to the player at position
   * [i, j]
   * @param source
   * @param i
   * @param j
   */
  void connectSourceToNewVideo(const VideoFile &source, int i, int j);

 signals:
  void getinfo();

 public slots:
  /**
   * @brief on_recordButton_clicked GUI
   */
  void on_recordButton_clicked();

  /**
   * @brief on_playButton_clicked Plays or pauses all players, depending on
   * current state.
   */
  void on_playButton_clicked();

  void updateRecordingList();

  /**
   * @brief continueUpdateClientList updates the Client List in the GUI
   */
  void continueUpdateClientList();

  /**
   * @brief on_stopButton_clicked Stops the playback of all players.
   */
  void on_stopButton_clicked();

  void openRecording(QListWidgetItem *item);

  /**
   * @brief videoPlayerOpenOptions Opens an option dialog for setting the
   * geometry of a player.
   * @param index Index of the player to be done changes to
   * Called when the video player emits its playerDelete signal.
   */
  void videoPlayerOpenOptions(quint8 index);

  /**
   * @brief videoPlayerDelete Deletes a video player.
   * @param index Index of the player to be deleted
   */
  void videoPlayerDelete(quint8 index);

  /**
   * @brief videoPlayerDelete Deletes a video player.
   * @param index Index of the player to be deleted
   */
  void videoPlayerDeleteAlsoInGrid(quint8 index);

  /**
   * @brief saveFile save a File
   */
  void saveFile();
  /**
   * @brief settings Settings of the program
   */
  void settingsDialogButton();
  /**
   * @brief about Information about the program
   */
  void about();
  /**
   * @brief closeWindow Close MainWindow
   */
  void closeWindow();

  void saveRecording();
 private slots:
  /**
   * @brief on_pushButton_Percent_clicked GUI toggle Percent in LEDs
   */
  void on_pushButton_Percent_clicked();
  /**
   * @brief on_pushButton_clicked toggle log size
   */
  void on_pushButton_clicked();

 private:
  void recordStart();
  void recordStop();
  void printClients();

  QColor getColorFromPercent(int percent);

  /**
   * @brief getFreePlayerId Finds an ID that is not used by another player at
   * the moment.
   * @return The free ID
   */
  quint8 getFreePlayerId();

  void clearVideoPlayers();

  PlaybackView *playbackView;
};

#endif  // MAINWINDOW_H
