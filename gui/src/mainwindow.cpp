#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>

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

  /* --- RECORD-TAB: table set-up --- */
  ui->tableWidget->setColumnCount(2);
  ui->tableWidget->setRowCount(1);
  ui->tableWidget->setColumnWidth(0, 160);
  ui->tableWidget->setColumnWidth(1, 20);
  ui->tableWidget->setShowGrid(false);

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

  // Row-Count auf 0 setzen, damit bei mehrmaligem Wiederholen der
  // Aktualisierung, die Liste nur so lange ist,wie sie Elemente hat.
  ui->tableWidget->setRowCount(0);
  // Eine Liste von Clients durchlaufen
  for (int i = 0; i < server->findChildren<MyThread *>().size(); i++) {
    /* In das tableWidget neue tableItems erstellen. Links Clientname.
 * Rechts Leerer String, der später eingefärbt wird.
 * WICHTIG: Erhöhen der rowCount!
 */

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    int SocketDescriptor =
        server->findChildren<MyThread *>()
            .at(i)
            ->socketDescriptor;  // Socket-Descriptor, int der
    // die Socket eindeutig
    // identifiziert
    QTcpSocket *socket =
        server->findChildren<MyThread *>()
            .at(i)
            ->socket;  // Socket-Pointer, verweist auf das Socket-Object
    QHostInfo HI = QHostInfo::fromName(
        socket->peerAddress()
            .toString());  // Host-Info/-Name. Funktioniert noch
    // nicht wie es soll.
    ui->tableWidget->setItem(
        i, 0, new QTableWidgetItem(QString::number(SocketDescriptor)
                                       .append(" ")
                                       .append(HI.hostName())));
    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));

    // Dummyvergleiche fuer mehr bunte Farben.
    if (server->findChildren<MyThread *>().at(i)->socketDescriptor > 18) {
      ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("red"));
    } else if (server->findChildren<MyThread *>().at(i)->socketDescriptor >
               10) {
      ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("yellow"));
    } else {
      ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("green"));
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
