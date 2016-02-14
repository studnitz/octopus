#include "playbackview.h"
#include "mainwindow.h"

PlaybackView::PlaybackView(QObject *parent) : QObject(parent) {}

void PlaybackView::openRecording(QListWidgetItem *item) {}

void PlaybackView::playAllPlayers() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;

  QList<QMediaPlayer *>::iterator i;
  if (!player->empty()) switch (player->at(0)->state()) {
      case QMediaPlayer::PlayingState:
        for (i = player->begin(); i != player->end(); ++i) {
          (*i)->pause();
        }
        //ui->playButton->setText("Play");
        p->log("Pausiere Wiedergabe der Aufnahme");
        break;
      default:
        for (i = player->begin(); i != player->end(); ++i) {
          (*i)->play();
        }
        if (player->at(0)->state() == QMediaPlayer::PlayingState) {
          //ui->playButton->setText("Pause");
          p->log("Starte Wiedergabe der Aufnahme");
        }
        break;
    }
}

void PlaybackView::stopAllPlayers()
{
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;

  QList<QMediaPlayer *>::iterator i;
  for (i = player->begin(); i != player->end(); ++i) {
    (*i)->stop();
  }
  p->log("Stoppe Wiedergabe der Aufnahme");
}

void PlaybackView::updateRecordingList(QListWidget *list)
{
  list->clear();
  QStringList nameFilter("*.off");
  QDir dir = QDir("/home/tosz/git/build-octopus-Desktop_Qt_5_5_1_GCC_64bit-Debug/gui");
  QStringList offFiles = dir.entryList(nameFilter);
  foreach (QString file, offFiles) { list->addItem(file); }

}

void PlaybackView::videoPlayerDelete(quint8 index)
{
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;
  QList<VideoPlayer *> *videoPlayer = p->videoPlayer;

  videoPlayer->at(index)->disconnect();
  delete videoPlayer->at(index);
  delete player->at(index);
  QList<VideoPlayer *>::iterator i;
  for (i = videoPlayer->begin() + index; i != videoPlayer->end(); ++i) {
    (*i)->index -= 1;
  }
  player->removeAt(index);
  videoPlayer->removeAt(index);
}

void PlaybackView::videoPlayerDeleteAlsoInGrid(quint8 index, Grid *grid)
{
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<VideoPlayer *> *videoPlayer = p->videoPlayer;

  QPair<int, int> pos = grid->getVideoFilePositionById(videoPlayer->at(index)->videoFileId);
  grid->deleteSource(pos.first, pos.second);

  videoPlayerDelete(index);
}
