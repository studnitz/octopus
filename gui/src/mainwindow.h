#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../server/src/server.h"
#include "../server/src/recording.h"

#include <QSettings>
#include "settingsdialog.h"
#include "playersettingsdialog.h"

// Videoplayer
#include <QMediaPlayer>
#include <QListWidget>

#include "videoplayer.h"
#include "recordingview.h"
#include "playbackview.h"
#include "guiinterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  /**
 * @brief recordingView subclass for recording capability
 */
  RecordingView *recordingView;

  /**
   * @brief playbackView subclass for playback capability
   */
  PlaybackView *playbackView;

  /**
   * @brief showPercentage flag for showing percentage in recordingView
   */
  bool showPercentage = 0;

  /**
   * @brief versionOctopus current version# of the octopus-program
   */
  QString versionOctopus = "0.314a";  // Versionnumber

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

  /**
   * @brief loadPlayersFromRecording creates video-output for video-files.
   * Amount of players created depends on recording (.off-file)
   */
  void loadPlayersFromRecording();

  /**
   * @brief log Helper function for writing info messages into the log section.
   * @param msg QString that holds the message
   */
  void log(QString msg);

  /**
   * @brief recording points to a Recording-class which is beeing created from
   * an .off-file
   */
  Recording *recording;

  /**
   * @brief connectSourceToNewVideo Connects source to the player at position
   * [i, j]
   * @param source Videofile to be connected to player
   * @param i x-coordinate in grid
   * @param j y-coordinate in grid
   */
  void connectSourceToNewVideo(const VideoFile &source, int i, int j);

  void tryConnection(QString serverIP);
signals:

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

  /**
   * @brief updateRecordingList updates the list of recordings with .off-files
   * contained in the directory of the executable
   */
  void updateRecordingList();

  /**
   * @brief continueUpdateClientList updates the Client List in the GUI
   */
  void continueUpdateClientList();

  /**
   * @brief on_stopButton_clicked Stops the playback of all players.
   */
  void on_stopButton_clicked();

  /**
   * @brief openRecording loads a recording. Also calls
   * loadPlayersFromRecording().
   * @param item
   */
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
  /**
   * @brief getColorFromPercent creates a Color ranging from green over yellow
   * to red, depending on percent. 100% is deep red, while 0% is green.
   * @param percent percentage
   * @return QColor ranging from green over yellow to red
   */
  QColor getColorFromPercent(int percent);

  /**
   * @brief ui pointer to the complete UI
   */
  Ui::MainWindow *ui;

  /**
   * @brief getFreePlayerId Finds an ID that is not used by another player at
   * the moment.
   * @return The free ID
   */
  quint8 getFreePlayerId();

  /**
   * @brief clearVideoPlayers deletes all videoplayers on the video-output layer
   * only. Does not change the grid-structure.
   */
  void clearVideoPlayers();

  /**
   * @brief guiInterface Interface to communicate with ServerPI
   */
  GUIInterface *guiInterface;
};

#endif  // MAINWINDOW_H
