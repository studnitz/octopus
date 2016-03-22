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
 * MainWindow handier and more overseeable. For a detailed description of the
 * methods go to the MainWindow-class where a method with the same name for each
 * method of this class exists.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
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

 public slots:
  void videoPlayerDeleteAlsoInGrid(quint8 index);
};

#endif  // PLAYBACKVIEW_H
