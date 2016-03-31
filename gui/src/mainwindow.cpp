#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostInfo>
#include <QDir>
#include <QtWidgets/QLabel>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowTitle(
      QString("octopus: Vernetztes Videocapture Tool ").append(versionOctopus));

  // Initalizing the table
  ui->tableWidget->setColumnCount(5);
  ui->tableWidget->setRowCount(1);
  ui->tableWidget->setColumnWidth(1, 20);
  ui->tableWidget->setColumnWidth(2, 20);
  ui->tableWidget->setColumnWidth(3, 20);
  ui->tableWidget->setColumnWidth(4, 70);
  ui->tableWidget->horizontalHeader()->setSectionResizeMode(
      0, QHeaderView::Stretch);
  ui->tableWidget->setShowGrid(false);

  // Menubar init
  QMenu *menuFile = ui->menuBar->addMenu(tr("Datei"));
  QAction *speichern = new QAction(tr("Speichern"), this);
  menuFile->addAction(speichern);
  QAction *exportieren = new QAction(tr("Exportieren"), this);
  menuFile->addAction(exportieren);
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
  connect(exportieren, SIGNAL(triggered()), this,
          SLOT(exportierenDialogButton()));
  connect(about, SIGNAL(triggered()), this, SLOT(about()));
  connect(close, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->recordingList, &QListWidget::itemDoubleClicked, this,
          &MainWindow::openRecording);
  connect(ui->saveRecordingButton, &QPushButton::clicked, this,
          &MainWindow::saveRecording);
  connect(ui->reloadButton, &QPushButton::clicked, this,
          &MainWindow::updateRecordingList);
  connect(ui->rebootClientsButton, &QPushButton::clicked, this,
          &MainWindow::rebootAllClients);
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::continueUpdateClientList);
  timer->start(50);

  /* --- PLAY-TAB: videoplayer set-up --- */
  player = new QList<QMediaPlayer *>();
  videoPlayer = new QList<VideoPlayer *>();

  playbackView = new PlaybackView(this);
  recordingView = new RecordingView(this, ui->tab);
  connect(timer, &QTimer::timeout, recordingView, &RecordingView::updateGrid);
  QString serverIP = this->settings->value("octopus/ServerIP").toString();
  tryConnection(serverIP);
}

void MainWindow::tryConnection(QString serverIP) {
  QHostAddress addr = QHostAddress(serverIP);
  quint16 port = 1235;
  guiInterface = new GUIInterface(addr, port, this);

  guiInterface->tryConnect(addr, port);
  if (guiInterface->socket->waitForConnected(2000)) {
    qDebug() << "GUI Interface connected";
    connect(guiInterface->socket, &QTcpSocket::readyRead, guiInterface,
            &GUIInterface::receiveData);
    connect(guiInterface, &GUIInterface::deviceListUpdated, recordingView,
            &RecordingView::updateVideoDeviceList);
  } else {
    qDebug() << "GUI Interface could not connect to Server Interface";

    bool ok;
    QString text = QInputDialog::getText(
        this, tr("Connection Error, server adress not found"),
        tr("IP des Servers:"), QLineEdit::Normal, "127.0.0.1", &ok);
    if (ok && !text.isEmpty()) {
      tryConnection(text);
      this->settings->setValue("octopus/ServerIP", text);
    }
  }
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
  recordingView->record_button(ui->recordButton);
}

void MainWindow::saveFile() { saveRecording(); }

void MainWindow::settingsDialogButton() {
  SettingsDialog *sD = new SettingsDialog();
  sD->show();
}

void MainWindow::rebootAllClients() {
  QMessageBox::StandardButtons buttons = QMessageBox::Ok | QMessageBox::Cancel;
  QMessageBox::StandardButton answer = QMessageBox::question(
      this, "Confirmation", "Wollen Sie wirklich alle Clients neustarten?", buttons);
  if (answer == QMessageBox::Ok) {
      QJsonObject data;
      guiInterface->sendData("reboot", data);
      qDebug() << "Rebooting all Clients!";
  }
}

void MainWindow::exportierenDialogButton() {
  ExportierenDialog *eD = new ExportierenDialog(this);
  connect(guiInterface, &GUIInterface::exportIsFinished, eD,
          &ExportierenDialog::exportFinished);
  connect(guiInterface, &GUIInterface::exportErrored, eD,
          &ExportierenDialog::exportErrored);
  eD->show();
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
  QJsonObject data;
  data["data"] = "looool";
  guiInterface->sendData("getInfo", data);

  // Initialize RowCount with 0
  ui->tableWidget->setRowCount(0);
  QStringList headerLabels;
  headerLabels << "Name"
               << "D"
               << "M"
               << "C"
               << "T";
  ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
  ui->tableWidget->horizontalHeaderItem(1)->setToolTip("Disk");
  ui->tableWidget->horizontalHeaderItem(2)->setToolTip("Memory");
  ui->tableWidget->horizontalHeaderItem(3)->setToolTip("CPU");
  ui->tableWidget->horizontalHeaderItem(4)->setToolTip("Time");
  ui->tableWidget->horizontalHeader()->show();

  // Update each Row/Client. Iterator can't be used here because index is needed
  for (int i = 0; i < guiInterface->clients->length(); i++) {
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    ClientGui *client = guiInterface->clients->at(i);
    ui->tableWidget->setItem(
        i, 0,
        new QTableWidgetItem(QString::number(i).append(" " + client->name)));
    // get ClientInfos
    int DiskUsage = client->disk;
    int MemUsage = client->mem;
    int CPUUsage = client->cpu;
    QString time = client->currentTime;

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

    //     Update 'LED' of Time
    ui->tableWidget->setItem(i, 4, new QTableWidgetItem(time));
    ui->tableWidget->item(i, 4)->setToolTip("Time: " + time);
    ui->tableWidget->item(i, 4)->setFont(QFont("Arial", 8));
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

void MainWindow::on_playButton_clicked() { playbackView->playAllPlayers(); }

void MainWindow::updateRecordingList() {
  playbackView->updateRecordingList(ui->recordingList);
}

void MainWindow::log(QString msg) {
  ui->debugTextEdit->setText(
      msg.append("\n").append(ui->debugTextEdit->toPlainText()));
}

void MainWindow::on_stopButton_clicked() { playbackView->stopAllPlayers(); }

void MainWindow::videoPlayerOpenOptions(quint8 index) {
  PlayerSettingsDialog *psDialog = new PlayerSettingsDialog(index, this);
  psDialog->exec();

  psDialog->deleteLater();
}

void MainWindow::videoPlayerDelete(quint8 index) {
  playbackView->videoPlayerDelete(index);
}

void MainWindow::videoPlayerDeleteAlsoInGrid(quint8 index) {
  playbackView->videoPlayerDeleteAlsoInGrid(index);
}

quint8 MainWindow::getFreePlayerId() {
  return playbackView->getFreePlayerId(videoPlayer);
}

void MainWindow::loadPlayersFromRecording() {
  playbackView->loadPlayersFromRecording(videoPlayer, recording);
}

void MainWindow::clearVideoPlayers() {
  playbackView->clearVideoPlayers(videoPlayer);
}

void MainWindow::connectSourceToNewVideo(const VideoFile &source, int i,
                                         int j) {
  playbackView->connectSourceToNewVideo(source, videoPlayer, ui->frame_6, i, j);
}

void MainWindow::openRecording(QListWidgetItem *item) {
  playbackView->openRecording(item);
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

void MainWindow::on_recordStopButton_clicked() {
  QString data2("");
  QJsonObject data;
  data["data"] = data2;
  guiInterface->sendData("stopCameras", data);
  log("Aufnahme stoppen");
}

void MainWindow::on_pauseButton_clicked() { playbackView->pauseAllPlayers(); }
