#include "playersettingsdialog.h"
#include "ui_playersettingsdialog.h"
#include "src/mainwindow.h"

PlayerSettingsDialog::PlayerSettingsDialog(quint8 index, QWidget *parent)
    : QDialog(parent), ui(new Ui::PlayerSettingsDialog) {
  ui->setupUi(this);
  this->setWindowTitle(QString("Einstellungen vom Player mit ID=")
                           .append(QString::number(index)));

  MainWindow *p = qobject_cast<MainWindow *>(parent);

  QPair<int, int> posPair =
      p->recording->grid.getVideoFilePositionById(p->videoPlayer->at(index)->videoFileId);

  ui->spinBox_2->setValue(posPair.first);
  ui->spinBox_4->setValue(posPair.second);

  ui->spinBox->setValue(1);
  ui->spinBox_3->setValue(1);
  ui->spinBox->setEnabled(false);
  ui->spinBox_3->setEnabled(false);
}

PlayerSettingsDialog::~PlayerSettingsDialog() { delete ui; }
