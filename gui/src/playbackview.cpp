#include "playbackview.h"
#include "mainwindow.h"
#include <QStyle>
#include <QToolButton>

PlaybackView::PlaybackView(QObject *parent) : QObject(parent) {}

void PlaybackView::playAllPlayers() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;

  QList<QMediaPlayer *>::iterator i;
  if (!player->empty()) {
      for (i = player->begin(); i != player->end(); ++i) {
        (*i)->play();
      }
      if (player->at(0)->state() == QMediaPlayer::PlayingState) {
        // ui->playButton->setText("Pause");
        p->log("Starte Wiedergabe der Aufnahme");
      }
  }
}

void PlaybackView::pauseAllPlayers() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;

  QList<QMediaPlayer *>::iterator i;
  if (!player->empty()) {
    for (i = player->begin(); i != player->end(); ++i) {
      (*i)->pause();
    }
    // ui->playButton->setText("Play");
    p->log("Pausiere Wiedergabe der Aufnahme");
  }
}

void PlaybackView::stopAllPlayers() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;

  QList<QMediaPlayer *>::iterator i;
  for (i = player->begin(); i != player->end(); ++i) {
    (*i)->stop();
  }
  p->log("Stoppe Wiedergabe der Aufnahme");
}

void PlaybackView::updateRecordingList(QListWidget *list) {
  list->clear();
  QStringList nameFilter("*.off");
  QDir dir = QDir::current();
  dir.cd("recordings");
  QStringList offFiles = dir.entryList(nameFilter);
  foreach (QString file, offFiles) { list->addItem(file); }
}

void PlaybackView::videoPlayerDelete(quint8 index) {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<QMediaPlayer *> *player = p->player;
  QList<VideoPlayer *> *videoPlayer = p->videoPlayer;

  player->at(index)->stop();
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

void PlaybackView::videoPlayerDeleteAlsoInGrid(quint8 index) {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QList<VideoPlayer *> *videoPlayer = p->videoPlayer;
  Grid *grid = &p->recording->grid;

  QPair<int, int> pos =
      grid->getVideoFilePositionById(videoPlayer->at(index)->videoFileId);
  grid->deleteSource(pos.second, pos.first);

  videoPlayerDelete(index);
}

void PlaybackView::clearVideoPlayers(QList<VideoPlayer *> *videoPlayer) {
  for (int i = videoPlayer->length() - 1; i >= 0; --i) {
    videoPlayerDelete(i);
  }
}

void PlaybackView::connectSourceToNewVideo(const VideoFile &source,
                                           QList<VideoPlayer *> *videoPlayer,
                                           QFrame *canvas, int i, int j) {
  // ---- Adjustable parameters
  quint16 initialMarginX = 10;
  quint16 initialMarginY = 10;
  qint16 marginX = 1;
  qint16 marginY = 1;
  quint8 playerRatioX = 16, playerRatioY = 9;
  // -------------------------------

  quint16 playerWidth, playerHeight;
  quint16 playerGridPosX = i;
  quint16 playerGridPosY = j;
  quint8 playerIndex = getFreePlayerId(videoPlayer);
  MainWindow *parent = qobject_cast<MainWindow *>(this->parent());

  double playerWidthDouble = ((double)(canvas->width() - 2 * initialMarginX) /
                              parent->recording->grid.grid.length());
  double playerHeightDouble = ((double)(canvas->height() - 2 * initialMarginY) /
                               parent->recording->grid.grid.at(0).length());

  // Determine if size should be height or width oriented depending
  // on window-size, so players are in always visible area
  if (qRound(playerWidthDouble / playerRatioX) <
      qRound(playerHeightDouble / playerRatioY)) {
    playerWidth = qRound(playerWidthDouble);
    playerHeight = qRound((playerWidthDouble * playerRatioY) / playerRatioX);
  } else {
    playerHeight = qRound(playerHeightDouble);
    playerWidth = qRound((playerHeightDouble * playerRatioX) / playerRatioY);
  }

  quint16 playerPosX =
      playerWidth * playerGridPosX + initialMarginX + marginX * playerGridPosX;
  quint16 playerPosY =
      playerHeight * playerGridPosY + initialMarginY + marginY * playerGridPosY;

  // Create Player
  parent->player->append(new QMediaPlayer(this));
  videoPlayer->append(new VideoPlayer(playerIndex, source.id, canvas));

  // Apply new position
  parent->player->at(playerIndex)->setVideoOutput(videoPlayer->at(playerIndex));
  videoPlayer->at(playerIndex)->move(playerPosX, playerPosY);
  videoPlayer->at(playerIndex)->resize(playerWidth, playerHeight);
  videoPlayer->at(playerIndex)->show();
  QDir dir = QDir::current();
  dir.cd("recordings");
  // Connect Source
  parent->player->at(playerIndex)
      ->setMedia(QUrl::fromLocalFile(dir.absoluteFilePath(source.filepath)));

  // Connect Signals
  connect(videoPlayer->at(playerIndex), &VideoPlayer::playerOpenOptions, parent,
          &MainWindow::videoPlayerOpenOptions);
  connect(videoPlayer->at(playerIndex), &VideoPlayer::playerDelete, this,
          &PlaybackView::videoPlayerDeleteAlsoInGrid);
}

void PlaybackView::openRecording(QListWidgetItem *item) {
  MainWindow *parent = qobject_cast<MainWindow *>(this->parent());
  if (!parent->player->empty()) delete parent->recording;
  parent->recording = new Recording();
  QDir fullDir = QDir::current();
  fullDir.cd("recordings");
  QString fullPath = fullDir.absoluteFilePath(item->text());

  qDebug() << "opened Recording: " << fullPath;
  parent->recording->loadRecording(fullPath);
  parent->loadPlayersFromRecording();

  connect((parent->player->first()), SIGNAL(positionChanged(qint64)), parent,
          SLOT(positionChanged(qint64)));
  connect((parent->player->first()), SIGNAL(durationChanged(qint64)), parent,
          SLOT(durationChanged(qint64)));

}

void PlaybackView::loadPlayersFromRecording(QList<VideoPlayer *> *videoPlayer,
                                            Recording *recording) {
  MainWindow *parent = qobject_cast<MainWindow *>(this->parent());
  clearVideoPlayers(videoPlayer);
  // Durchlaufen des Rasters im Recording und suche nach Sources
  for (int i = 0; i < recording->grid.grid.length(); ++i) {
    for (int j = 0; j < recording->grid.grid.at(0).length(); ++j) {
      // Wenn aktuelle VideoFile nicht leer ist
      if (recording->grid.grid.at(i).at(j).id != 0) {
        parent->connectSourceToNewVideo(recording->grid.grid.at(i).at(j), i, j);
      }
    }
  }
}

quint8 PlaybackView::getFreePlayerId(const QList<VideoPlayer *> *videoPlayer) {
  if (videoPlayer->empty()) return 0;
  return videoPlayer->last()->index + 1;
}
