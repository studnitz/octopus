#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QList>
#include <QMessageBox>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisierung der Tabelle
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,160);
    ui->tableWidget->setColumnWidth(1,20);
    ui->tableWidget->setShowGrid(false);

    // Initialisierung der Clients-Liste
    clients = new QList<QString>();
    clients->reserve(100);
    checkForClients();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recordButton_clicked()
{
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
    //Ui::TClient *test1, *test2;
    //test1 = new Ui::TClient{"Client 1", "asdf"};

    QMessageBox qmsg;



    //clients << test1;
    //clients->insert(0, &test1);
    //clients[0] = &test1;
    //clients->insert(0,QString("Client 1"));
    clients->insert(clients->size(),"Client 2");

    qmsg.setText(QString::number(clients->size()));
    qmsg.exec();
    //clients << QString("Client1") << QString("Client2");

    checkForClients();
}

void MainWindow::checkForClients() {
    /* some magic to check for clients */

    for (int i = 0; i < clients->size(); i++) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(clients->at(i)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));
        ui->tableWidget->item(i,1)->setBackgroundColor(QColor("red"));
    }

}
