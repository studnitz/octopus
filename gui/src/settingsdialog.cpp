#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsDialog) {
  ui->setupUi(this);

  int settingIntervall = settings->value("octopus/Intervall").toInt();
  int settingQuality = settings->value("octopus/Quality").toInt();
  int settingGridWidth = settings->value("octopus/GridWidth").toInt();
  int settingGridHeigth = settings->value("octopus/GridHeigth").toInt();
  int settingLocation = settings->value("octopus/Location").toInt();
  QString Ip = settings->value("octopus/ServerIP").toString();

  ui->updateIntervall->addItem(tr("0.5 Sekunden"));
  ui->updateIntervall->addItem(tr("1 Sekunde"));
  ui->updateIntervall->addItem(tr("2 Sekunden"));
  ui->updateIntervall->addItem(tr("5 Sekunden"));
  ui->updateIntervall->addItem(tr("10 Sekunden"));
  ui->updateIntervall->addItem(tr("20 Sekunden"));
  ui->updateIntervall->setCurrentIndex(settingIntervall);

  ui->recordQuality->addItem(tr("144p"));
  ui->recordQuality->addItem(tr("240p"));
  ui->recordQuality->addItem(tr("360p"));
  ui->recordQuality->addItem(tr("720p"));
  ui->recordQuality->addItem(tr("1080p"));
  ui->recordQuality->setCurrentIndex(settingQuality);

  ui->recordStorage->addItem(tr("SD-Karte der Clients"));
  ui->recordStorage->addItem(tr("SD-Karte des Servers"));
  ui->recordStorage->addItem(tr("Festplatte"));
  ui->recordStorage->addItem(tr("Clients, dann auf Server holen"));
  ui->recordStorage->addItem(tr("Clients, dann auf Festplatte holen"));
  ui->recordStorage->setCurrentIndex(settingLocation);

  ui->settingsOther->addItem("Platzhalter");
  ui->settingsOther->addItem("für");
  ui->settingsOther->addItem("andere");
  ui->settingsOther->addItem("Optionen");

  if (Ip.compare("")==0) {
    ui->lineEditIP->setText("192.168.1.1");
  } else {
    ui->lineEditIP->setText(Ip);
  }
  ui->gridWidth->setMaximum(9999);
  ui->gridWidth->setValue(settingGridWidth);
  ui->gridHeigth->setMaximum(9999);
  ui->gridHeigth->setValue(settingGridHeigth);
}

SettingsDialog::~SettingsDialog() { delete ui; }

void SettingsDialog::on_buttonBox_accepted() {
  settings->setValue("octopus/Intervall", ui->updateIntervall->currentIndex());
  settings->setValue("octopus/Quality", ui->recordQuality->currentIndex());
  settings->setValue("octopus/Location", ui->recordStorage->currentIndex());
  settings->setValue("octopus/GridWidth", ui->gridWidth->value());
  settings->setValue("octopus/GridHeigth", ui->gridHeigth->value());
  settings->setValue("octopus/ServerIP", ui->lineEditIP->text());
  this->close();
}

void SettingsDialog::on_buttonBox_rejected() { this->close(); }
