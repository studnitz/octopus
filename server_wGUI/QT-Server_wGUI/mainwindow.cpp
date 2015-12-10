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
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnWidth(0,160);
    ui->tableWidget->setColumnWidth(1,20);
    ui->tableWidget->setShowGrid(false);

    // Initialisierung der Clients-Liste
    clients = new QList<QString>();
    clients->reserve(100);
    // Testdaten
    clients->insert(clients->size(),"Client 1");
    clients->insert(clients->size(),"Client 2");
    clients->insert(clients->size(),"Client 3");
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

    printClients();
}

void MainWindow::printClients() {
    /* some magic to check for clients */

    /*QTableWidgetItem *i1, *i2;
    i1 = new QTableWidgetItem(clients->at(0));
    i2 = new QTableWidgetItem(QString::number(0));
    ui->tableWidget->setItem(0,0,i1);
    ui->tableWidget->setItem(0,1,i2);

    i1 = new QTableWidgetItem(clients->at(1));
    i2 = new QTableWidgetItem(QString::number(1));
    ui->tableWidget->setItem(1,0,i1);
    ui->tableWidget->setItem(1,1,i2);*/


    for (int i = 0; i < clients->size(); i++) {

        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(clients->at(i)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));
        ui->tableWidget->item(i,1)->setBackgroundColor(QColor("red"));
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    }

}
