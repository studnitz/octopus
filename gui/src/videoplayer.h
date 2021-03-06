#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QVideoWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>

/**
 * @brief The VideoPlayer class inherits from QVideoWidget and is a display
 * level class used for the playback of a VideoFile. Each VideoPlayer needs a
 * QMediaPlayer to store the video-stream and perform actions on it (like play,
 * pause etc). VideoPlayer is really only the display widget.
 * @author Bartosz Milejski, Yannick Schädle, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class VideoPlayer : public QVideoWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructor for a new VideoPlayer. For the choice on which index to
   * take, use PlaybackView::getVideoPlayerId to find the lowest possible index
   * and use it for best performance.
   * @param index index of the new VideoPlayer.
   * @param videoFileId the id of the VideoFile-object that should be played in
   * this VideoPlayer. Note this has NOTHING to do with the index
   * @param parent Best choose the same frame as parent for all the players
   */
  VideoPlayer(quint8 index, quint32 videoFileId, QWidget *parent = 0);

  /**
   * @brief contextMenuEvent is an event handler for context-menu opening. A
   * context menu is created and a following option chosen.
   * @param e e contains details about the occured event needed for evaluation
   *
   * @bug Sometimes, when the amount of VideoPlayers created is nearly the
   * maximum allowed by the Grid, a click on deletePlayer or on Settings in the
   * context menu makes the programm crash. We haven't had the time to analyze
   * patterns yet, the bug occurs on linux and windows though. When it occured,
   * it was also pretty strange that it occured on 2 from 4 created players (2*2
   * grid), on the players with ID=2 and ID=3, so it doesn't seem like an
   * off-by-one bug...
   */
  void contextMenuEvent(QContextMenuEvent *e);

  /**
   * @brief index contains the unique ID of the player.
   */
  quint8 index;

  /**
   * @brief videoFileId holds the ID of the Videofile that VideoPlayer is
   * supposed to play.
   */
  quint32 videoFileId;

 signals:

  /**
   * @brief playerOpenOptions is fired when 'Einstellungen' in the context menu
   * is clicked.
   * @param index ID of the player
   */
  void playerOpenOptions(quint8 index);

  /**
   * @brief playerDelete is fired when 'Player löschen' in the context menu is
   * clicked.
   * @param index ID of the player
   */
  void playerDelete(quint8 index);
};

#endif  // VIDEOPLAYER_H
