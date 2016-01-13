#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>
#include <QMessageBox>
#include <stdio.h>
#include <QHostInfo>

//Videoplayer
#include <QtWidgets>    // Laden der Dateien (QDir)
#include <QDialog>      // Einstellungs-Dialog (QDialog)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* --- SET-UP Tabelle --- */
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnWidth(0,160);
    ui->tableWidget->setColumnWidth(1,20);
    ui->tableWidget->setShowGrid(false);

    /* --- SET-UP Videoplayer --- */
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(player);
    videoPlayer = new VideoPlayer(ui->tab_2);
    player->setVideoOutput(videoPlayer);

    videoPlayer->resize(320,240);
    videoPlayer->move(10,10);
    videoPlayer->show();

    // Videoplayer Slots verbinden
    connect(videoPlayer, SIGNAL(playerClicked(int)), this, SLOT(videoPlayerClicked(int)));


    /* --- SET-UP Playlist --- */
    playlistModel = new PlaylistModel(this);
    ui->listView->setModel(playlistModel);
}

MainWindow::~MainWindow() {
    videoPlayer->disconnect();
    delete ui;
}

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

  // Row-Count auf 0 setzen, damit bei mehrmaligem Wiederholen der
  // Aktualisierung, die Liste nur so lange ist,wie sie Elemente hat.
  ui->tableWidget->setRowCount(0);
  // Eine Liste von Clients durchlaufen
  for (int i = 0; i < server->findChildren<MyThread*>().size(); i++) {
    /* In das tableWidget neue tableItems erstellen. Links Clientname.
 * Rechts Leerer String, der später eingefärbt wird.
 * WICHTIG: Erhöhen der rowCount!
 */

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
    int SocketDescriptor =
        server->findChildren<MyThread*>()
            .at(i)
            ->socketDescriptor;  // Socket-Descriptor, int der
    // die Socket eindeutig
    // identifiziert
    QTcpSocket* socket =
        server->findChildren<MyThread*>()
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
    if (server->findChildren<MyThread*>().at(i)->socketDescriptor > 18) {
      ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("red"));
    } else if (server->findChildren<MyThread*>().at(i)->socketDescriptor > 10) {
      ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("yellow"));
    } else {
      ui->tableWidget->item(i, 1)->setBackgroundColor(QColor("green"));
    }
  }
}

/**
 *  Pausiert Aufnahme oder spielt sie ab, abhängig von PlayingState.
 *  Verändert außerdem die Button-Beschriftung.
 */
void MainWindow::on_playButton_clicked()
{
    switch(player->state()) {
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
void MainWindow::on_openFileButton_clicked()
{
    // Öffnen der Aufzeichnungen
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open movies"),QDir::currentPath());

    if (!fileNames.empty()) {
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
        ui->listView->setCurrentIndex(playlistModel->index(playlist->currentIndex(), 0));
    }
}

/**
 * Hilfsfunktion zum Füllen des Debug-Fensters.
 */
void MainWindow::log(QString msg)
{
    ui->debugTextEdit->setText(msg.append("\n").append(ui->debugTextEdit->toPlainText()));
}

/**
 * Stoppen der aktuellen Aufnahme.
 */
void MainWindow::on_stopButton_clicked()
{
    player->stop();
    log("Stoppe Wiedergabe der Aufnahme");
}

/**
 * Auswählen der Aufnahme, die abgespielt werden soll.
 */
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    player->setMedia(playlist->media(index.row()));
}

void MainWindow::videoPlayerClicked(int index)
{
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

}

void MainWindow::optDialogSaveClicked(int posX, int posY)
{
    videoPlayer->move(posX, posY);
}
