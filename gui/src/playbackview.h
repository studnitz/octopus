#ifndef PLAYBACKVIEW_H
#define PLAYBACKVIEW_H

#include <QObject>
#include <QtGui>
#include <QListWidget>
#include <QMediaPlayer>
#include <../server/src/recording.h>
#include <../server/src/grid.h>
#include <src/videoplayer.h>

/**
 * @brief The PlaybackView class
 * Class to handle the elements in the playback tab.
 */
class PlaybackView : public QObject {
  Q_OBJECT
 public:
  explicit PlaybackView(QObject *parent = 0);

  void playAllPlayers();
  void stopAllPlayers();
  void updateRecordingList(QListWidget *list);
  void videoPlayerDelete(quint8 index);
  void clearVideoPlayers(QList<VideoPlayer *> *videoPlayer);
  void connectSourceToNewVideo(const VideoFile &source,
                               QList<VideoPlayer *> *videoPlayer,
                               QFrame *canvas, int i, int j);
  void openRecording(QListWidgetItem *item);
  void loadPlayersFromRecording(QList<VideoPlayer *> *videoPlayer,
                                Recording *recording);
  quint8 getFreePlayerId(const QList<VideoPlayer *> *videoPlayer);

 private:
 signals:

 public slots:
  void videoPlayerDeleteAlsoInGrid(quint8 index);
};

#endif  // PLAYBACKVIEW_H
