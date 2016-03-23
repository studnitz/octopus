#include "exportierendialog.h"
#include "ui_exportierendialog.h"
#include "mainwindow.h"

ExportierenDialog::ExportierenDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ExportierenDialog) {
  ui->setupUi(this);
  ui->qualityComboBox->addItem("480p");
  ui->qualityComboBox->addItem("720p");
  ui->codecComboBox->addItem("h264");
  ui->progressBar->setValue(0);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &ExportierenDialog::new_progress);
}

ExportierenDialog::~ExportierenDialog() { delete ui; }

void ExportierenDialog::on_startButton_clicked() {
  QString quality = ui->qualityComboBox->currentText();
  QString codec = ui->codecComboBox->currentText();
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  p->guiInterface->startExport(quality, codec);
  timer->start(500);
  p->guiInterface->exportStatus = 0;
}

void ExportierenDialog::new_progress() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  ui->progressBar->setValue(p->guiInterface->exportStatus);
  p->guiInterface->getExportStatus();
}
