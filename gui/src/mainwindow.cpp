#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>
#include <QMessageBox>
#include <stdio.h>
#include <QHostInfo>
#include <cstdlib>
#include <ctime>

// Videoplayer
#include <QtWidgets>
#include <QVideoWidget>
#include <QVideoSurfaceFormat>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "playlistmodel.h"

QMediaPlayer *player;
PlaylistModel *playlistModel;
QMediaPlaylist *playlist;
QVideoWidget *videoWidget;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // Initialisierung der Tabelle
  ui->tableWidget->setColumnCount(4);
  ui->tableWidget->setRowCount(1);
  ui->tableWidget->setColumnWidth(0, 149);
  ui->tableWidget->setColumnWidth(1, 20);
  ui->tableWidget->setColumnWidth(2, 20);
  ui->tableWidget->setColumnWidth(3, 20);
  ui->tableWidget->setShowGrid(false);

  // Videoplayer-Setup
  player = new QMediaPlayer(this);
  playlist = new QMediaPlaylist(player);
  videoWidget = new QVideoWidget(ui->tab_2);
  player->setVideoOutput(videoWidget);

  videoWidget->move(50, 50);
  videoWidget->resize(320, 240);
  videoWidget->show();

  // Playlist-Setup
  playlistModel = new PlaylistModel(this);
  ui->listView->setModel(playlistModel);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_recordButton_clicked() {
  /*
 * Button ändert die Schrift, wenn man ihn anklickt
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
  qDebug()<< "start 1";
  emit this->getinfo();

}


void MainWindow::continueUpdateClientList(){
    qDebug()<<"gui:: back in gui 7";
    // Row-Count auf 0 setzen, damit bei mehrmaligem Wiederholen der
    // Aktualisierung, die Liste nur so lange ist,wie sie Elemente hat.
      ui->tableWidget->setRowCount(0);
      // Eine Liste von Clients durchlaufen
      for (int i = 0; i < server->getClients().size(); i++) {
        /* In das tableWidget neue tableItems erstellen. Links Clientname.
     * Rechts Leerer String, der später eingefärbt wird.
     * WICHTIG: Erhöhen der rowCount!
     */

        // emit getinfo();
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

        QHostInfo HI = QHostInfo::fromName(
            server->getClients()
                .at(i)
                ->ClientIP);  // Host-Info/-Name. Funktioniert noch
        // nicht wie es soll.
        ui->tableWidget->setItem(
            i, 0, new QTableWidgetItem(
                      QString::number(i).append(" ").append(HI.hostName())));


        int DiskUsage = server->getClients().at(i)->ClientInfo[2];
        int MemUsage = server->getClients().at(i)->ClientInfo[1];
        int CPUUsage = server->getClients().at(i)->ClientInfo[0];

        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));
        ui->tableWidget->item(i, 1)->setToolTip(
            "Disk Usage " +
            QString::number(DiskUsage));
        qDebug() << DiskUsage << "   " << MemUsage << "   " << CPUUsage;
        if (DiskUsage > 75) {
          ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("red"));
        } else if (DiskUsage> 50) {
          ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("yellow"));
        } else {
          ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("green"));
        }

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(""));
        ui->tableWidget->item(i, 2)->setToolTip(
            "RAM Usage " +
            QString::number(MemUsage));
        if (MemUsage > 75) {
          ui->tableWidget->item(i, 2)->setBackgroundColor(QColor("red"));
        } else if (MemUsage > 50) {
          ui->tableWidget->item(i, 2)->setBackgroundColor(QColor("yellow"));
        } else {
          ui->tableWidget->item(i, 2)->setBackgroundColor(QColor("green"));
        }

        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(""));
        ui->tableWidget->item(i, 3)->setToolTip(
            "CPU Usage " +
            QString::number(CPUUsage));
        if (CPUUsage > 75) {
          ui->tableWidget->item(i, 3)->setBackgroundColor(QColor("red"));
        } else if (CPUUsage > 50) {
          ui->tableWidget->item(i, 3)->setBackgroundColor(QColor("yellow"));
        } else {
          ui->tableWidget->item(i, 3)->setBackgroundColor(QColor("green"));
        }
      }
}


/**
 *  Pausiert Aufnahme oder spielt sie ab, abhängig von PlayingState.
 *  Verändert außerdem die Button-Beschriftung.
 */
void MainWindow::on_playButton_clicked() {
  switch (player->state()) {
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
  }
}

/**
 * Button bietet Funktionalität zum Öffnen von Aufnahmen.
 */
void MainWindow::on_openFileButton_clicked() {
  // Öffnen der Aufzeichnungen
  QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open movies"),
                                                        QDir::currentPath());
  // Erste der Aufzeichnung automatisch in Player laden
  player->setMedia(QUrl::fromLocalFile(fileNames.at(0)));

  // Aufzeichnungen in Playlist laden
  for (int i = 0; i < fileNames.size(); i++) {
    playlist->addMedia(QUrl::fromLocalFile(fileNames.at(i)));
    log(QString("Lade ").append(fileNames.at(i)).append("..."));
  }
  playlist->setCurrentIndex(playlist->mediaCount());

  // PlaylistModel befüllen mit Playlist-Inhalt
  playlistModel->setPlaylist(playlist);

  // Indexbereich der Liste aktualisieren
  ui->listView->setCurrentIndex(
      playlistModel->index(playlist->currentIndex(), 0));
}

/**
 * Hilfsfunktion zum Füllen des Debug-Fensters.
 */
void MainWindow::log(QString msg) {
  ui->debugTextEdit->setText(
      msg.append("\n").append(ui->debugTextEdit->toPlainText()));
}

/**
 * Stoppen der aktuellen Aufnahme.
 */
void MainWindow::on_stopButton_clicked() {
  player->stop();
  log("Stoppe Wiedergabe der Aufnahme");
}

/**
 * Auswählen der Aufnahme, die abgespielt werden soll.
 */
void MainWindow::on_listView_doubleClicked(const QModelIndex &index) {
  player->setMedia(playlist->media(index.row()));
}
