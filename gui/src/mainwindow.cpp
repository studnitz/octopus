#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>
#include <QHostInfo>
#include <QDir>

// Videoplayer
#include <QtWidgets>
#include <QDialog>
#include <QMediaPlaylist>
#include "playlistmodel.h"

PlaylistModel *playlistModel;
QMediaPlaylist *playlist;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowTitle(
      QString("octopus: Vernetztes Videocapture Tool ").append(versionOctopus));
  // Initalizing the table
  ui->tableWidget->setColumnCount(4);
  ui->tableWidget->setRowCount(1);
  ui->tableWidget->setColumnWidth(1, 20);
  ui->tableWidget->setColumnWidth(2, 20);
  ui->tableWidget->setColumnWidth(3, 20);
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::Stretch);
  ui->tableWidget->setShowGrid(false);

  // Menubar init
  QMenu *menuFile = ui->menuBar->addMenu(tr("Datei"));
  QAction *speichern = new QAction(tr("Speichern"), this);
  menuFile->addAction(speichern);
  QMenu *menuEdit = ui->menuBar->addMenu(tr("Bearbeiten"));
  QAction *settings = new QAction(tr("Einstellungen"), this);
  menuEdit->addAction(settings);
  QMenu *menuExtras = ui->menuBar->addMenu(tr("Extras"));
  QAction *about = new QAction(tr("Über"), this);
  menuExtras->addAction(about);
  QAction *close = new QAction(tr("Schließen"), this);
  menuFile->addAction(close);
  updateRecordingList();
  connect(speichern, SIGNAL(triggered()), this, SLOT(saveFile()));
  connect(settings, SIGNAL(triggered()), this, SLOT(settingsDialogButton()));
  connect(about, SIGNAL(triggered()), this, SLOT(about()));
  connect(close, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->recordingList, &QListWidget::itemDoubleClicked, this,
          &MainWindow::openRecording);
  connect(ui->saveRecordingButton, &QPushButton::clicked, this,
          &MainWindow::saveRecording);
  connect(ui->reloadButton, &QPushButton::clicked, this,
          &MainWindow::updateRecordingList);
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(continueUpdateClientList()));
  timer->start(1000);

  /* --- PLAY-TAB: videoplayer set-up --- */
  player = new QList<QMediaPlayer *>();
  videoPlayer = new QList<VideoPlayer *>();
  playlist = new QMediaPlaylist();
  playlistModel = new PlaylistModel(this);
  ui->listView->setModel(playlistModel);
}

MainWindow::~MainWindow() {
  QList<VideoPlayer *>::iterator i;
  for (i = videoPlayer->begin(); i != videoPlayer->end(); ++i) {
    (*i)->disconnect();
  }
  delete ui;
  delete settings;
}

void MainWindow::on_recordButton_clicked() {
  static bool startStopFlag = false;
  if (startStopFlag == false) {
    ui->recordButton->setText("Stop");
    recordStart();
    startStopFlag = true;
  } else {
    ui->recordButton->setText("Aufnehmen");
    recordStop();
    startStopFlag = false;
  }
}

// TODO functionality
void MainWindow::saveFile() { qDebug() << "save"; }

void MainWindow::settingsDialogButton() {
  SettingsDialog *sD = new SettingsDialog();
  sD->show();
}

void MainWindow::about() {
  // Create dialog
  QDialog *aboutDialog = new QDialog(this);
  aboutDialog->resize(300, 150);
  aboutDialog->setWindowTitle(QString("Über"));
  aboutDialog->move(this->x() + 250, this->y() + 300);

  // Create info label
  QLabel *aboutLabel =
      new QLabel(QString("Octopus: Vernetztes Video Capturetool\nVersion ")
                     .append(versionOctopus),
                 aboutDialog);
  aboutLabel->move(10, 10);

  // Create save button and connect functinality
  QPushButton *closeButton = new QPushButton("Schließen", aboutDialog);
  closeButton->move(115, 120);
  connect(closeButton, &QPushButton::pressed,
          [this, aboutDialog]() { aboutDialog->close(); });
  aboutDialog->exec();

  closeButton->disconnect();
  aboutDialog->deleteLater();
}

void MainWindow::closeWindow() { this->close(); }

void MainWindow::recordStart() {
  /* magic */

  server->recordLocally();
  log("Aufzeichnung wurde gestartet.");
}

void MainWindow::recordStop() {
  /* magic */

  server->stopRecordings();
  log("Aufzeichnung wurde gestoppt.");
}

void MainWindow::printClients() {
  /* some magic to check for clients */
  qDebug() << "start 1";
  emit this->getinfo();
}

QColor MainWindow::getColorFromPercent(int percent) {
  int red = 0;
  int green = 0;
  // between 50 an 100 red is 255
  // between 0 an 50 red slowly increases
  red = percent < 50 ? 255 * (2 * percent / 100.0) : 255;
  // between 0 and 50 green is 255
  // between 50 and 100 green slowly decreases
  green = percent > 50 ? 255 - (255 * ((2 * percent - 100) / 100.0)) : 255;

  // QColor from RGB, A (Brightness) is default 255
  return QColor(red, green, 0);
}

void MainWindow::continueUpdateClientList() {
  // emit signal to get new Infos
  emit this->getinfo();

  // Initialize RowCount with 0
  ui->tableWidget->setRowCount(0);
  QStringList headerLabels;
  headerLabels << "Name"
               << "D"
               << "M"
               << "C";
  ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
  ui->tableWidget->horizontalHeaderItem(1)->setToolTip("Disk");
  ui->tableWidget->horizontalHeaderItem(2)->setToolTip("Memory");
  ui->tableWidget->horizontalHeaderItem(3)->setToolTip("CPU");
  ui->tableWidget->horizontalHeader()->show();

  // Update each Row/Client. Iterator cant be used here because index is needed
  for (int i = 0; i < server->getClients().size(); i++) {
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

    QHostInfo HI = QHostInfo::fromName(
        server->getClients()
            .at(i)
            ->ClientIP);  // Host-Info/-Name. Not really working as it should
    ui->tableWidget->setItem(
        i, 0, new QTableWidgetItem(
                  QString::number(i).append(" ").append(HI.hostName())));
    // get ClientInfos
    int DiskUsage = server->getClients().at(i)->ClientInfo[2];
    int MemUsage = server->getClients().at(i)->ClientInfo[0];
    int CPUUsage = server->getClients().at(i)->ClientInfo[1];

    // Update 'LED' of DiskUsage
    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));
    ui->tableWidget->item(i, 1)
        ->setToolTip("Disk Usage: " + QString::number(DiskUsage) + "%");
    ui->tableWidget->item(i, 1)
        ->setBackgroundColor(getColorFromPercent(DiskUsage));
    ui->tableWidget->item(i, 1)->setFont(QFont("Arial", 8));

    // Update 'LED' of MemUsage
    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
    ui->tableWidget->item(i, 2)
        ->setToolTip("RAM Usage: " + QString::number(MemUsage) + "%");
    ui->tableWidget->item(i, 2)
        ->setBackgroundColor(getColorFromPercent(MemUsage));
    ui->tableWidget->item(i, 2)->setFont(QFont("Arial", 8));

    // Update 'LED' of CPUUsage
    ui->tableWidget->setItem(i, 3, new QTableWidgetItem(""));
    ui->tableWidget->item(i, 3)
        ->setToolTip("CPU Usage: " + QString::number(CPUUsage) + "%");
    ui->tableWidget->item(i, 3)
        ->setBackgroundColor(getColorFromPercent(CPUUsage));
    ui->tableWidget->item(i, 3)->setFont(QFont("Arial", 8));

    // Adds Usage in Percent to the 'LED'
    if (showPercentage) {
      ui->tableWidget->item(i, 1)->setText(QString::number(DiskUsage));
      ui->tableWidget->item(i, 2)->setText(QString::number(MemUsage));
      ui->tableWidget->item(i, 3)->setText(QString::number(CPUUsage));
      if (DiskUsage > 70) {
        ui->tableWidget->item(i, 1)->setTextColor(QColor("white"));
      } else {
        ui->tableWidget->item(i, 1)->setTextColor(QColor("black"));
      }
      if (MemUsage > 70) {
        ui->tableWidget->item(i, 2)->setTextColor(QColor("white"));
      } else {
        ui->tableWidget->item(i, 2)->setTextColor(QColor("black"));
      }
      if (CPUUsage > 70) {
        ui->tableWidget->item(i, 3)->setTextColor(QColor("white"));
      } else {
        ui->tableWidget->item(i, 3)->setTextColor(QColor("black"));
      }
    }
  }
}

void MainWindow::on_playButton_clicked() {
  QList<QMediaPlayer *>::iterator i;
  if (!player->empty()) switch (player->at(0)->state()) {
      case QMediaPlayer::PlayingState:
        for (i = player->begin(); i != player->end(); ++i) {
          (*i)->pause();
        }
        ui->playButton->setText("Play");
        log("Pausiere Wiedergabe der Aufnahme");
        break;
      default:
        for (i = player->begin(); i != player->end(); ++i) {
          (*i)->play();
        }
        if (player->at(0)->state() == QMediaPlayer::PlayingState) {
          ui->playButton->setText("Pause");
          log("Starte Wiedergabe der Aufnahme");
        }
        break;
    }
}

void MainWindow::updateRecordingList() {
  ui->recordingList->clear();
  QStringList nameFilter("*.off");
  QDir dir = QDir("/home/snx/build-octopus-Desktop-Debug/server");
  QStringList offFiles = dir.entryList(nameFilter);
  foreach (QString file, offFiles) { ui->recordingList->addItem(file); }
}

void MainWindow::on_openFileButton_clicked() {
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open movies"),
                                                        QDir::currentPath());
  if (!fileNames.empty()) {
    QList<QMediaPlayer *>::iterator j;
    // Load first recording into players automatically
    for (j = player->begin(); j != player->end(); ++j) {
      (*j)->setMedia(QUrl::fromLocalFile(fileNames.at(0)));
    }

    // Fill playlist with files
    QStringList::iterator i;
    for (i = fileNames.begin(); i != fileNames.end(); ++i) {
      playlist->addMedia(QUrl::fromLocalFile(*i));
      log(QString("Lade ").append(*i).append("..."));
    }
    playlist->setCurrentIndex(playlist->mediaCount());

    // Link playlist, playlistModel and listView
    playlistModel->setPlaylist(playlist);
    ui->listView->setCurrentIndex(
        playlistModel->index(playlist->currentIndex(), 0));
  }
}

void MainWindow::log(QString msg) {
  ui->debugTextEdit->setText(
      msg.append("\n").append(ui->debugTextEdit->toPlainText()));
}

void MainWindow::on_stopButton_clicked() {
  QList<QMediaPlayer *>::iterator i;
  for (i = player->begin(); i != player->end(); ++i) {
    (*i)->stop();
  }
  log("Stoppe Wiedergabe der Aufnahme");
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index) {
  QList<QMediaPlayer *>::iterator i;
  for (i = player->begin(); i != player->end(); ++i) {
    (*i)->setMedia(playlist->media(index.row()));
  }
}

void MainWindow::videoPlayerOpenOptions(quint8 index) {
  PlayerSettingsDialog *psDialog = new PlayerSettingsDialog(index, this);
  psDialog->exec();

  psDialog->deleteLater();
}

void MainWindow::videoPlayerDelete(quint8 index) {
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

void MainWindow::videoPlayerDeleteAlsoInGrid(quint8 index) {
  QPair<int, int> pos = recording->grid.getVideoFilePositionById(videoPlayer->at(index)->videoFileId);
  recording->grid.deleteSource(pos.first, pos.second);

  videoPlayerDelete(index);
}

void MainWindow::on_addPlayerButton_clicked() {
  loadPlayersFromRecording();
}

quint8 MainWindow::getFreePlayerId() {
  if (videoPlayer->empty()) return 0;
  return videoPlayer->last()->index + 1;
}

void MainWindow::loadPlayersFromRecording() {
  clearVideoPlayers();
  // Durchlaufen des Rasters im Recording und suche nach Sources
  for (int i = 0; i < recording->grid.grid.length(); ++i) {
    for (int j = 0; j < recording->grid.grid.at(0).length(); ++j) {
      // Wenn aktuelle VideoFile nicht leer ist
      if (recording->grid.grid.at(i).at(j).id != 0) {
        connectSourceToNewVideo(recording->grid.grid.at(i).at(j), i, j);
      }
    }
  }
}

void MainWindow::clearVideoPlayers() {
  for (int i = videoPlayer->length()-1; i >= 0 ; --i) {
      videoPlayerDelete(i);
    }
}

void MainWindow::connectSourceToNewVideo(const VideoFile &source, int i,
                                         int j) {
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
  quint8 playerIndex = getFreePlayerId();

  double playerWidthDouble =
      ((double)(ui->frame_6->width() - 2 * initialMarginX) /
       recording->grid.grid.length());
  double playerHeightDouble =
      ((double)(ui->frame_6->height() - 2 * initialMarginY) /
       recording->grid.grid.at(0).length());

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
  player->append(new QMediaPlayer(this));
  videoPlayer->append(new VideoPlayer(playerIndex, source.id, ui->frame_6));

  // Apply new position
  player->at(playerIndex)->setVideoOutput(videoPlayer->at(playerIndex));
  videoPlayer->at(playerIndex)->move(playerPosX, playerPosY);
  videoPlayer->at(playerIndex)->resize(playerWidth, playerHeight);
  videoPlayer->at(playerIndex)->show();

  // Connect Source
  qDebug() << source.filepath;
  player->at(playerIndex)->setMedia(QUrl::fromLocalFile(source.filepath));

  // Connect Signals
  connect(videoPlayer->at(playerIndex), &VideoPlayer::playerOpenOptions, this,
          &MainWindow::videoPlayerOpenOptions);
  connect(videoPlayer->at(playerIndex), &VideoPlayer::playerDelete, this,
          &MainWindow::videoPlayerDeleteAlsoInGrid);
}

void MainWindow::openRecording(QListWidgetItem *item) {
  recording = new Recording();
  QString fullPath =
      QDir::homePath() + "/build-octopus-Desktop-Debug/server/" + item->text();
  qDebug() << "opened Recording: " << fullPath;
  recording->loadRecording(fullPath);
  loadPlayersFromRecording();
}

void MainWindow::saveRecording() { recording->saveRecording(); }


void MainWindow::on_pushButton_Percent_clicked() {
  showPercentage = !showPercentage;
  continueUpdateClientList();
}

void MainWindow::on_pushButton_clicked() {
  if (ui->pushButton->text() == "+") {
    ui->debugTextEdit->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Expanding);
    ui->pushButton->setText("-");
  } else {
    ui->debugTextEdit->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Minimum);
    ui->pushButton->setText("+");
  }
  ui->debugTextEdit->adjustSize();
}
