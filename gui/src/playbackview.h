#ifndef PLAYBACKVIEW_H
#define PLAYBACKVIEW_H

#include <QTimer>
#include <QDir>
#include <QListWidget>
#include <QMediaPlayer>
#include <../server/src/recording.h>
#include "videoplayer.h"

/**
 * @brief The PlaybackView class stores the functionality from MainWindow that
 * is needed for the whole playback-tab. It was once introduced to make
 * MainWindow handier and more overseeable. For a more detailed description of
 * the methods go to the MainWindow-class where a method with the same name for
 * each method of this class exists.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class PlaybackView : public QObject {
  Q_OBJECT
 public:
  explicit PlaybackView(QObject *parent = 0);

  /**
   * @brief playAllPlayers loops through all the QMediaPlayers and either pauses
   * all of them if they are playing right now, or makes them play if they are
   * all paused right now.
   */
  void playAllPlayers();

  /**
   * @brief stopAllPlayers loops through all the QMediaPlayers and stops them
   * all.
   */
  void stopAllPlayers();

  /**
   * @brief updateRecordingList updates the list of recordings with .off files.
   * The files are beeing searched in a subfolder to the executable octopus
   * file, called recordings.
   * @param list The QListWidget to be updated
   */
  void updateRecordingList(QListWidget *list);

  /**
   * @brief videoPlayerDelete deletes a video player. Is used for refreshing
   * because the grid is not changed.
   * @param index Index of the player to be deleted
   */
  void videoPlayerDelete(quint8 index);

  /**
   * @brief clearVideoPlayers deletes all VideoPlayer on the video-output layer
   * only. Does not change the grid-structure. Basically it only loops through
   * the list of VideoPlayers and calls videoPlayerDelete on each and every one
   * of them.
   * @param videoPlayer The list of VideoPlayers to be cleared
   */
  void clearVideoPlayers(QList<VideoPlayer *> *videoPlayer);

  /**
   * @brief connectSourceToNewVideo connects source to the player at position
   * [i, j]. It also creates this QMediaPlayer and VideoPlayer, calculates where
   * to print the player and does this as well.
   * @param source VideoFile to be connected to player
   * @param i x-coordinate in grid
   * @param j y-coordinate in grid
   */
  void connectSourceToNewVideo(const VideoFile &source,
                               QList<VideoPlayer *> *videoPlayer,
                               QFrame *canvas, int i, int j);

  /**
   * @brief openRecording loads a Recording from an .off file and calls
   * loadPlayersFromRecording after that, so the VideoPlayers are created as
   * well.
   * @param item represents the .off file to be loaded
   */
  void openRecording(QListWidgetItem *item);

  /**
   * @brief loadPlayersFromRecording needs a set Recording. It can then execute
   * connectSourceToNewVideo for every file in the given recording. So to sum it
   * up this method reads a Recording and creates VideoPlayers, QMediaPlayers
   * and connects signals and slots on them for the amount of players contained
   * in the Recording. The Recording was beforehand read from an .off file or
   * set otherwise.
   * @param videoPlayer A QList of VideoPlayer-pointers that the VideoPlayers
   * will be created into
   * @param recording The already existing recording to be read from
   */
  void loadPlayersFromRecording(QList<VideoPlayer *> *videoPlayer,
                                Recording *recording);

  /**
   * @brief getFreePlayerId finds the lowest ID that is not used by another
   * player at the moment.
   * @param videoPlayer The list of VideoPlayers that is used for the lookup
   * @return A free unique ID
   */
  quint8 getFreePlayerId(const QList<VideoPlayer *> *videoPlayer);

 public slots:

  /**
   * @brief videoPlayerDelete deletes a VideoPlayer, its corresponding
   * QMediaPlayer and changes the grid. Restoring the VideoPlayer or the Player
   * with ID=index is not possible after calling this method.
   * @param index Index of the player to be deleted
   */
  void videoPlayerDeleteAlsoInGrid(quint8 index);
};

#endif  // PLAYBACKVIEW_H
