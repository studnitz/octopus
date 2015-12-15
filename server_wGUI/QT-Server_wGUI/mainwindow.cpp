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
    for (int i = 0; i < clients->size(); i++) {
        /* In das tableWidget neue tableItems erstellen. Links Clientname.
         * Rechts Leerer String, der später eingefärbt wird.
         * WICHTIG: Erhöhen der rowCount!
         */
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(clients->at(i)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(""));
        if (clients->at(i).contains('1')) {
            ui->tableWidget->item(i,1)->setBackgroundColor(QColor("red"));
        } else if (clients->at(i).contains("2")) {
            ui->tableWidget->item(i,1)->setBackgroundColor(QColor("yellow"));
        } else {
            ui->tableWidget->item(i,1)->setBackgroundColor(QColor("green"));
        }
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    }

}
