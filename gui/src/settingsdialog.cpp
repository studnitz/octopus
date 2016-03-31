#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);

  int settingQuality = settings->value(("octopus/Quality")).toInt();
  int settingGridWidth = settings->value("octopus/GridWidth").toInt();
  int settingGridHeigth = settings->value("octopus/GridHeigth").toInt();
  int settingLocation = settings->value("octopus/Location").toInt();
  int settingFps = settings->value("octopus/Fps").toInt();
  QString settingLocationData;
  QString Ip = settings->value("octopus/ServerIP").toString();


  ui->recordQuality->addItem(tr("320x240"));
  ui->recordQuality->addItem(tr("640x480"));
  ui->recordQuality->addItem(tr("1280x960"));
  ui->recordQuality->setCurrentIndex(settingQuality);

  ui->recordFps->addItem(tr("30"));
  ui->recordFps->addItem(tr("15"));

  int recordindex = settingFps == 30 ? 0 : 1;
  ui->recordFps->setCurrentIndex(recordindex);

  ui->recordStorage->addItem(tr("Nur auf dem Server"));
  ui->recordStorage->addItem(
      tr("Server, dann auf GUI holen (Pfad auswählen...)"));
  // If custom path was already chosen once, than add that option as well
  if (settings->value("octopus/LocationData").isValid()) {
    settingLocationData = settings->value("octopus/LocationData").toString();
    ui->recordStorage->addItem(
        QString("Server, dann holen nach " + settingLocationData));
  }
  ui->recordStorage->setCurrentIndex(settingLocation);

  if (Ip.compare("") == 0) {
    ui->lineEditIP->setText("192.168.1.1");
  } else {
    ui->lineEditIP->setText(Ip);
  }
  ui->gridWidth->setMaximum(20);
  ui->gridWidth->setValue(settingGridWidth);
  ui->gridHeigth->setMaximum(20);
  ui->gridHeigth->setValue(settingGridHeigth);
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::on_buttonBox_accepted() {
  settings->setValue("octopus/Quality", ui->recordQuality->currentIndex());
  settings->setValue("octopus/Location", ui->recordStorage->currentIndex());
  settings->setValue("octopus/Fps", ui->recordFps->currentText().toInt());
  settings->setValue("octopus/GridWidth", ui->gridWidth->value());
  settings->setValue("octopus/GridHeigth", ui->gridHeigth->value());
  settings->setValue("octopus/ServerIP", ui->lineEditIP->text());
  this->close();
}

void SettingsDialog::on_buttonBox_rejected() { this->close(); }

void SettingsDialog::on_recordStorage_activated(int index) {
  // Set custom filepath for recordings saving
  if (index == 1) {
    QFileDialog filedialog(this);
    filedialog.setFileMode(QFileDialog::Directory);
    filedialog.setOption(QFileDialog::ShowDirsOnly, true);
    filedialog.setViewMode(QFileDialog::List);
    QString dir;
    if (filedialog.exec()) {
      dir = filedialog.selectedFiles().first();
      settings->setValue("octopus/LocationData", dir);
      ui->recordStorage->removeItem(ui->recordStorage->count() - 1);
      ui->recordStorage->addItem(QString("Server, dann holen nach " + dir));
      ui->recordStorage->setCurrentIndex(ui->recordStorage->count() - 1);
    }
  }
}
