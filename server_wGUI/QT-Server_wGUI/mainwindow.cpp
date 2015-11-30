#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *item1 = new QStandardItem("test");
    QStandardItem *item2 = new QStandardItem("test2");
    model->appendRow(item1);
    model->appendRow(item2);

    QTableWidget *tview = ui->tableWidget->setModel(model);

    /*if (ui->listView->viewMode() == ListMode) {
        ui->debugTextEdit->insertPlainText("ListView ist in ListMode");
    } else {
        ui->debugTextEdit->insertPlainText("ListView ist in IconMode");
    }*/
}
