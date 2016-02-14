#ifndef PLAYBACKVIEW_H
#define PLAYBACKVIEW_H

#include <QObject>
#include <QtGui>
#include <QListWidget>
#include <QMediaPlayer>
#include <../server/src/recording.h>
#include <../server/src/grid.h>

class PlaybackView : public QObject
{
  Q_OBJECT
public:
  explicit PlaybackView(QObject *parent = 0);

  void openRecording(QListWidgetItem *item);
  void playAllPlayers();
  void stopAllPlayers();
  void updateRecordingList(QListWidget* list);
  void videoPlayerDelete(quint8 index);
  void videoPlayerDeleteAlsoInGrid(quint8 index, Grid* grid);

private:
  Recording* recording;

signals:

public slots:
};

#endif // PLAYBACKVIEW_H
