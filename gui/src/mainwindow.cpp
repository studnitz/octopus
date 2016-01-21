#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>
#include <QHostInfo>

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

  // Initalizing the table
  ui->tableWidget->setColumnCount(4);
  ui->tableWidget->setRowCount(1);
  ui->tableWidget->setColumnWidth(0, 149);
  ui->tableWidget->setColumnWidth(1, 20);
  ui->tableWidget->setColumnWidth(2, 20);
  ui->tableWidget->setColumnWidth(3, 20);
  ui->tableWidget->setShowGrid(false);

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

void MainWindow::recordStart() {
    /* magic */

    log("Aufzeichnung wurde gestartet.");
}

void MainWindow::recordStop() {
    /* magic */

    log("Aufzeichnung wurde gestoppt.");
}

void MainWindow::on_pushButton_2_clicked() { printClients(); }

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
  switch (player->at(0)->state()) {
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
  // Create dialog
  QDialog *optDialog = new QDialog(this);
  optDialog->resize(500, 150);
  optDialog->setWindowTitle(QString("Einstellungen vom Player mit ID=")
                                .append(QString::number(index)));
  optDialog->move(this->x() + 200, this->y() + 300);

  // Create info labels
  QLabel *posXLabel = new QLabel(QString("Position X:"), optDialog);
  QLabel *posYLabel = new QLabel(QString("Position Y:"), optDialog);
  QLabel *widthLabel = new QLabel(QString("Breite:"), optDialog);
  QLabel *heightLabel = new QLabel(QString("Höhe:"), optDialog);
  posXLabel->move(10, 10);
  posYLabel->move(120, 10);
  widthLabel->move(10, 75);
  heightLabel->move(120, 75);

  // Create spin boxes for data display and fill them
  QSpinBox *posXInput = new QSpinBox(optDialog);
  QSpinBox *posYInput = new QSpinBox(optDialog);
  QSpinBox *widthInput = new QSpinBox(optDialog);
  QSpinBox *heightInput = new QSpinBox(optDialog);
  posXInput->setMaximum(9999);
  posYInput->setMaximum(9999);
  widthInput->setMaximum(9999);
  heightInput->setMaximum(9999);
  posXInput->move(10, 30);
  posYInput->move(120, 30);
  widthInput->move(10, 95);
  heightInput->move(120, 95);
  posXInput->setValue(videoPlayer->at(index)->x());
  posYInput->setValue(videoPlayer->at(index)->y());
  widthInput->setValue(videoPlayer->at(index)->width());
  heightInput->setValue(videoPlayer->at(index)->height());

  // Create save button and connect functinality
  QPushButton *saveButton = new QPushButton("Speichern", optDialog);
  saveButton->move(400, 10);
  connect(saveButton, &QPushButton::pressed,
          [this, index, &posXInput, &posYInput, &widthInput, &heightInput]() {
            videoPlayer->at(index)
                ->move(posXInput->value(), posYInput->value());
            videoPlayer->at(index)
                ->resize(widthInput->value(), heightInput->value());
          });
  optDialog->exec();

  saveButton->disconnect();
  optDialog->deleteLater();
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

void MainWindow::on_addPlayerButton_clicked() {
  // ---- Adjustable parameters
  quint16 initialMarginX = 10;
  quint16 initialMarginY = 10;
  qint16 marginX = 1;
  qint16 marginY = 1;
  quint16 newWidth = 240;
  quint16 newHeight = 180;
  // -------------------------------

  quint16 newX, newY;
  quint8 index = getFreePlayerId();

  // Calculate new position
  if (index == 0) {
    newX = initialMarginX;
    newY = initialMarginY;
  } else {
    newX = videoPlayer->at(index - 1)->x();
    newY = videoPlayer->at(index - 1)->y();

    if (newY + videoPlayer->at(index - 1)->height() + newHeight >
        ui->tabWidget->height()) {
      newY = initialMarginY;
      newX += marginX + newWidth;
    } else {
      newY += marginY + newHeight;
    }
  }

  player->append(new QMediaPlayer(this));
  videoPlayer->append(new VideoPlayer(ui->tab_2, index));

  // Apply new position
  player->at(index)->setVideoOutput(videoPlayer->at(index));
  videoPlayer->at(index)->move(newX, newY);
  videoPlayer->at(index)->resize(newWidth, newHeight);
  videoPlayer->at(index)->show();

  // Connect videoPlayer slots
  connect(videoPlayer->at(index), &VideoPlayer::playerOpenOptions, this,
          &MainWindow::videoPlayerOpenOptions);
  connect(videoPlayer->at(index), &VideoPlayer::playerDelete, this,
          &MainWindow::videoPlayerDelete);
}

quint8 MainWindow::getFreePlayerId() {
  if (videoPlayer->empty()) return 0;
  return videoPlayer->last()->index + 1;
}

void MainWindow::on_pushButton_Percent_clicked()
{
    showPercentage = !showPercentage;
    continueUpdateClientList();
}
