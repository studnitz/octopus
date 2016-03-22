#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../server/src/server.h"
#include "../server/src/recording.h"

#include <QSettings>
#include "settingsdialog.h"
#include "playersettingsdialog.h"

#include <QMediaPlayer>
#include <QListWidget>

#include "videoplayer.h"
#include "recordingview.h"
#include "playbackview.h"
#include "guiinterface.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class is the frontend and main user interface of
 * octopus. It's code and therefore also its functionality was divided into the
 * RecordingView and PlaybackView classes. Since the GUI does not necessarily
 * have to be on the same device that the server is, the GUI has to communicate
 * with the Server. Becase the application should be reliable we decided for a
 * TCP-based communication. That's what GUI-Interface is for.
 *
 * The Recording-Tab contains the functionality for starting and stopping a
 * recording, as well as the list of clients and their status. The ClientList
 * however is still implemented in the MainWindow class since it doesn't really
 * have anything to do with the recording itself. Each Client in this list is
 * represented by ClientGui.
 *
 * The Playback-Tab contains a list of .off files (octopus looks for them in a
 * subfolder called recordings). Each .off file corresponds to a Recording. When
 * double-clicking on an .off file, the contained Recording is loaded and
 * VideoPlayer-objects are created. Those can be right-clicked on, so a context
 * menu openes. From there the user can delete players or change their position.
 * This is handled by PlayerSettingsDialog.
 *
 * A menu bar exists in the top of the window. The corresponding functionality
 * is beeing held by SettingsDialog.
 *
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for MainWindow. Self-explaining. No options needed.
   */
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  /**
   * @brief recordingView holds a pointer to the subclass that contains the
   * recording functionality.
   */
  RecordingView *recordingView;

  /**
   * @brief playbackView holds a pointer to the subclass that contains the
   * playback functionality.
   */
  PlaybackView *playbackView;

  /**
   * @brief showPercentage is a flag signalizing if the percentages in the
   * ClientGui-list in the recording-tab should be shown.
   */
  bool showPercentage = 0;

  /**
   * @brief versionOctopus holds the current version number of the
   * octopus-program
   */
  QString versionOctopus = "0.3141.a-unstable_alpha";

  /**
   * @brief settings stores a pointer to settings. The file itself is stored in
   * ~/.config/TUD\ IAD/octopus. On Windows it should be the same path, except ~
   * is replaced by the windows home directory.
   */
  QSettings *settings = new QSettings(QString("TUD IAD"), QString("octopus"));

  /**
   * @brief videoPlayer holds a list of VideoPlayer-pointers. The VideoPlayer
   * class inherits from QVideoWidget. It is the output object for the stream
   * that is decoded by the player object. Also it has some overridden events
   * compared to QVideoWidget.
   */
  QList<VideoPlayer *> *videoPlayer;

  /**
   * @brief player holds a list of QMediaPlayer-pointers. The players implement
   * the functionality for video-playback. They hold references to the files
   * and it's also them who get calls for stopping or playing the sources.
   */
  QList<QMediaPlayer *> *player;

  /**
   * @brief loadPlayersFromRecording needs a set Recording. It can then execute
   * connectSourceToNewVideo for every file in the given recording. So to sum it
   * up this method reads a Recording and creates VideoPlayers, QMediaPlayers
   * and connects signals and slots on them for the amount of players contained
   * in the Recording. The Recording was beforehand read from an .off file or
   * set otherwise.
   */
  void loadPlayersFromRecording();

  /**
   * @brief log is a helper function for writing info messages into the log
   * section at the bottom of the MainWindow.
   * @param msg QString that holds the message
   */
  void log(QString msg);

  /**
   * @brief recording points to a Recording-object which is beeing created from
   * an .off-file.
   */
  Recording *recording;

  /**
   * @brief connectSourceToNewVideo connects source to the player at position
   * [i, j]. It also creates this QMediaPlayer and VideoPlayer, calculates where
   * to print the player and does this as well.
   * @param source VideoFile to be connected to player
   * @param i x-coordinate in grid
   * @param j y-coordinate in grid
   */
  void connectSourceToNewVideo(const VideoFile &source, int i, int j);

  /**
   * @brief tryConnection tries to connect the GUI to the Server on the last IP
   * that a connection could be established on. If not, tryConnection is called
   * again on serverIP. Works recursively.
   * @param serverIP the IP to connect on if the initial connection does not
   * work
   */
  void tryConnection(QString serverIP);

  /**
   * @brief guiInterface holds a pointer to the interface that handles all the
   * communication with the Server.
   */
  GUIInterface *guiInterface;

 public slots:

  /**
   * @brief on_record_button is triggered when the Record-Button is clicked
   */
  void on_recordButton_clicked();

  /**
   * @brief on_playButton_clicked plays or pauses all players, depending on the
   * current state.
   */
  void on_playButton_clicked();

  /**
   * @brief updateRecordingList updates the list of recordings with .off-files
   * contained in the directory of the executable. Connected to the Reload
   * button.
   */
  void updateRecordingList();

  /**
   * @brief continueUpdateClientList updates the list of ClientGuis in the
   * recording-tab.
   */
  void continueUpdateClientList();

  /**
   * @brief on_stopButton_clicked stops the playback of all players.
   */
  void on_stopButton_clicked();

  /**
   * @brief openRecording loads a Recording from an .off file and calls
   * loadPlayersFromRecording after that, so the VideoPlayers are created as
   * well.
   * @param item represents the .off file to be loaded
   */
  void openRecording(QListWidgetItem *item);

  /**
   * @brief videoPlayerOpenOptions opens an option dialog for setting the
   * geometry of a player.
   * @param index Index of the player to be done changes to
   */
  void videoPlayerOpenOptions(quint8 index);

  /**
   * @brief videoPlayerDelete deletes a video player. Is used for refreshing
   * because the grid is not changed.
   * @param index Index of the player to be deleted
   */
  void videoPlayerDelete(quint8 index);

  /**
   * @brief videoPlayerDelete deletes a VideoPlayer, its corresponding
   * QMediaPlayer and changes the grid. Restoring the VideoPlayer or the Player
   * with ID=index is not possible after calling this method.
   * @param index Index of the player to be deleted
   */
  void videoPlayerDeleteAlsoInGrid(quint8 index);

  /**
   * @brief saveFile saves the Recording in its current states. All changes done
   * to the grid or the players are overwritten in the current .off file as
   * well.
   */
  void saveFile();

  /**
   * @brief Opens the settings-Dialog implemented by SettingsDialog.
   */
  void settingsDialogButton();

  /**
   * @brief about gives a brief overview about octopus
   */
  void about();

  /**
   * @brief closeWindow closes the MainWindow
   */
  void closeWindow();

  /**
   * @brief saveRecording saves all changes made to a Recording into the
   * corresponding .off file
   */
  void saveRecording();

 private slots:

  /**
   * @brief on_pushButton_Percent_clicked Make the GUI show the percentage in
   * status-LEDs
   */
  void on_pushButton_Percent_clicked();

  /**
   * @brief on_pushButton_clicked expand or shrink the log size depending on
   * the current state.
   */
  void on_pushButton_clicked();

  /**
   * @brief on_recordStopButton_clicked stop the current Recording
   */
  void on_recordStopButton_clicked();

 private:
  /**
   * @brief getColorFromPercent creates a color ranging from green over yellow
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
   * @brief getFreePlayerId finds the lowest ID that is not used by another
   * player at the moment.
   * @return The free ID
   */
  quint8 getFreePlayerId();

  /**
   * @brief clearVideoPlayers deletes all VideoPlayer on the video-output layer
   * only. Does not change the grid-structure. Basically it only loops through
   * the list of VideoPlayers and calls videoPlayerDelete on each and every one
   * of them.
   */
  void clearVideoPlayers();
};

#endif  // MAINWINDOW_H
