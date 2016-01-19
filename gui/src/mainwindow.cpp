#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>
#include <QMessageBox>
#include <stdio.h>
#include <QHostInfo>

//Videoplayer
#include <QtWidgets>
#include <QVideoWidget>
#include <QVideoSurfaceFormat>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "playlistmodel.h"


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

  // Videoplayer-Setup
  /*player = new QMediaPlayer(this);
  playlist = new QMediaPlaylist(player);
  videoWidget = new QVideoWidget(ui->tab_2);
  player->setVideoOutput(videoWidget);

  videoWidget->move(50, 50);
  videoWidget->resize(320, 240);
  videoWidget->show();

  // Playlist-Setup
  playlistModel = new PlaylistModel(this);
  ui->listView->setModel(playlistModel);*/
}

MainWindow::~MainWindow() {
  for (int i = 0; i < videoPlayer->size(); i++)
    videoPlayer->at(i)->disconnect();
  delete ui;
}

void MainWindow::on_recordButton_clicked() {
  /*
 * Button changes text, when its clicked
 */
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
        ->setToolTip("Disk Usage " + QString::number(DiskUsage));
    ui->tableWidget->item(i, 1)
        ->setBackgroundColor(getColorFromPercent(DiskUsage));
    ui->tableWidget->item(i, 1)->setFont(QFont("Arial", 8));

    // Update 'LED' of MemUsage
    ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
    ui->tableWidget->item(i, 2)
        ->setToolTip("RAM Usage " + QString::number(MemUsage));
    ui->tableWidget->item(i, 2)
        ->setBackgroundColor(getColorFromPercent(MemUsage));
    ui->tableWidget->item(i, 2)->setFont(QFont("Arial", 8));

    // Update 'LED' of CPUUsage
    ui->tableWidget->setItem(i, 3, new QTableWidgetItem(""));
    ui->tableWidget->item(i, 3)
        ->setToolTip("CPU Usage " + QString::number(CPUUsage));
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
  /*switch (player->state()) {
    case QMediaPlayer::PlayingState:
      player->pause();
      ui->playButton->setText("Play");
      log("Pausiere Wiedergabe der Aufnahme");
      break;
    default:
      player->play();
      if (player->state() == QMediaPlayer::PlayingState) {
        ui->playButton->setText("Pause");
        log("Starte Wiedergabe der Aufnahme");
      }
      break;
  }*/
}

void MainWindow::on_openFileButton_clicked() {
  // Öffnen der Aufzeichnungen
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open movies"),
                                                        QDir::currentPath());

  if (!fileNames.empty()) {
    // Erste der Aufzeichnung automatisch in Player laden
    // player->setMedia(QUrl::fromLocalFile(fileNames.at(0)));

    // Aufzeichnungen in Playlist laden
    for (int i = 0; i < fileNames.size(); i++) {
      playlist->addMedia(QUrl::fromLocalFile(fileNames.at(i)));
      log(QString("Lade ").append(fileNames.at(i)).append("..."));
    }
    playlist->setCurrentIndex(playlist->mediaCount());

    // PlaylistModel befüllen mit Playlist-Inhalt
    playlistModel->setPlaylist(playlist);

        // Indexbereich der Liste aktualisieren
        ui->listView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));
    }
}

void MainWindow::log(QString msg) {
  ui->debugTextEdit->setText(
      msg.append("\n").append(ui->debugTextEdit->toPlainText()));
}

void MainWindow::on_stopButton_clicked() {
  /*player->stop();*/
  log("Stoppe Wiedergabe der Aufnahme");
}

/**
 * Auswählen der Aufnahme, die abgespielt werden soll.
 */
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    //player->setMedia(playlist->media(index.row()));
}

void MainWindow::videoPlayerClicked(int index)
{
    /*
    // Dialogfenster erstellen
    QDialog *optDialog = new QDialog(this);
    optDialog->resize(500,150);
    optDialog->setWindowTitle(QString("Einstellungen vom Player mit ID=").append(QString::number(index)).append(""));
    optDialog->move(this->x()+200,this->y()+300);

    // Infotexte erstellen
    QLabel *posXLabel   = new QLabel(QString("Position X:"), optDialog);
    QLabel *posYLabel   = new QLabel(QString("Position Y:"), optDialog);
    QLabel *widthLabel  = new QLabel(QString("Breite:"));
    QLabel *heightLabel = new QLabel(QString("Höhe:"));
    posXLabel->move(10,10);
    posYLabel->move(120,10);
    widthLabel->move(10,75);
    heightLabel->move(120,75);

    // Eingabefenster erstellen und befüllen
    QSpinBox *posXInput     = new QSpinBox(optDialog);
    QSpinBox *posYInput     = new QSpinBox(optDialog);
    QSpinBox *widthInput    = new QSpinBox(optDialog);
    QSpinBox *heightInput   = new QSpinBox(optDialog);
    posXInput->setMaximum(9999);
    posYInput->setMaximum(9999);
    widthInput->setMaximum(9999);
    heightInput->setMaximum(9999);
    posXInput->move(10,35);
    posYInput->move(120,35);
    widthInput->move(10,90);
    heightInput->move(120,90);
    posXInput->setValue(videoPlayer->x());
    posYInput->setValue(videoPlayer->y());
    widthInput->setValue(videoPlayer->width());
    heightInput->setValue(videoPlayer->height());


    // Save-Button erstellen und connecten
    QPushButton *saveButton = new QPushButton("Speichern", optDialog);
    saveButton->move(400,10);
    connect(saveButton, &QPushButton::pressed,
            [this, &posXInput, &posYInput, &widthInput, &heightInput]() {
                videoPlayer->move(posXInput->value(), posYInput->value());
                videoPlayer->resize(widthInput->value(), heightInput->value());
    });

    optDialog->exec();

    // Connection trennen und Dialog zerstören
    saveButton->disconnect();
    optDialog->deleteLater();
    */
}

void MainWindow::on_addPlayerButton_clicked()
{
    // ---- Parameter variabel wählbar
    int initialMarginX = 10;
    int initialMarginY = 10;
    int marginX = 1;
    int marginY = 1;
    int newWidth = 240;
    int newHeight = 180;
    // -------------------------------

    int index = player->size();

    player->append(new QMediaPlayer(this));
    videoPlayer->append(new VideoPlayer(ui->tab_2, index));

    // Beim ersten Aufruf der Prozedur
    if (videoPlayer->size() == 1) {

        player->at(0)->setVideoOutput(videoPlayer->at(0));
        videoPlayer->at(0)->move(initialMarginX, initialMarginY);
        videoPlayer->at(0)->resize(newWidth, newHeight);
        videoPlayer->at(0)->show();
    // Sonst
    } else {
        int newX = videoPlayer->at(index-1)->x();
        int newY = videoPlayer->at(index-1)->y();

        if (newY + videoPlayer->at(index-1)->height() + newHeight > ui->tabWidget->height()) {
            newY = initialMarginY;
            newX += marginX + newWidth;
        } else {
            newY += marginY + newHeight;
        }

        player->at(index)->setVideoOutput(videoPlayer->at(index));
        videoPlayer->at(index)->move(newX, newY);
        videoPlayer->at(index)->resize(newWidth, newHeight);
        videoPlayer->at(index)->show();
    }
}

void MainWindow::on_pushButton_Percent_clicked()
{
    showPercentage = !showPercentage;
    continueUpdateClientList();
}
