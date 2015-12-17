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

QMediaPlayer *player;
QMediaPlaylist *playlist;
QVideoWidget *videoWidget;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisierung der Tabelle
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnWidth(0,160);
    ui->tableWidget->setColumnWidth(1,20);
    ui->tableWidget->setShowGrid(false);

    //Videoplayer-Tests
    player = new QMediaPlayer;
    playlist = new QMediaPlaylist(player);
    playlist->addMedia(QUrl("test.avi"));

    videoWidget = new QVideoWidget(ui->tab_2);
    player->setVideoOutput(videoWidget);

    videoWidget->move(50,50);
    videoWidget->resize(320,240);
    videoWidget->show();
    playlist->setCurrentIndex(1);
    player->play();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recordButton_clicked()
{
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
    ui->debugTextEdit->insertPlainText("Aufnahme wurde gestartet\n");
}

void MainWindow::recordStop() {
    ui->debugTextEdit->insertPlainText("Aufnahme wurde gestoppt\n");
}

void MainWindow::on_pushButton_2_clicked()
{
    printClients();
}

void MainWindow::printClients() {
    /* some magic to check for clients */

    // Eine Liste von Clients durchlaufen
    for (int i = 0; i < server->findChildren<MyThread*>().size(); i++) {
        /* In das tableWidget neue tableItems erstellen. Links Clientname.
         * Rechts Leerer String, der später eingefärbt wird.
         * WICHTIG: Erhöhen der rowCount!
         */

        int SocketDescriptor = server->findChildren<MyThread*>().at(i)->socketDescriptor;       //Socket-Descriptor, int der die Socket eindeutig identifiziert
        QTcpSocket* socket = server->findChildren<MyThread*>().at(i)->socket;                   //Socket-Pointer, verweist auf das Socket-Object
        QHostInfo HI = QHostInfo::fromName(socket->peerAddress().toString());                   //Host-Info/-Name. Funktioniert noch nicht wie es soll.
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(SocketDescriptor).append(" ").append(HI.hostName())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));

        //Dummyvergleiche fuer mehr bunte Farben.
        if (server->findChildren<MyThread*>().at(i)->socketDescriptor > 18) {
            ui->tableWidget->item(i,1)->setBackgroundColor(QColor("red"));
        } else if (server->findChildren<MyThread*>().at(i)->socketDescriptor > 10) {
            ui->tableWidget->item(i,1)->setBackgroundColor(QColor("yellow"));
        } else {
            ui->tableWidget->item(i,1)->setBackgroundColor(QColor("green"));
        }
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    }

}

void MainWindow::on_playButton_clicked()
{
    player->play();
}
