#include "playersettingsdialog.h"
#include "ui_playersettingsdialog.h"

#include <QMessageBox>
#include "src/mainwindow.h"

PlayerSettingsDialog::PlayerSettingsDialog(quint8 index, QWidget *parent)
    : QDialog(parent), ui(new Ui::PlayerSettingsDialog) {
  ui->setupUi(this);
  this->setWindowTitle(QString("Einstellungen vom Player mit ID=")
                           .append(QString::number(index)));

  MainWindow *p = qobject_cast<MainWindow *>(parent);
  playerIndex = index;
  changesDone = false;

  // Get position of clicked videoplayer
  QPair<int, int> posPair = p->recording->grid.getVideoFilePositionById(
      p->videoPlayer->at(index)->videoFileId);

  ui->spinBox_2->setValue(posPair.first);
  ui->spinBox_2->setMaximum(p->recording->grid.width - 1);
  ui->spinBox_4->setValue(posPair.second);
  ui->spinBox_4->setMaximum(p->recording->grid.height - 1);

  ui->spinBox->setValue(1);
  ui->spinBox_3->setValue(1);
  ui->spinBox->setEnabled(false);
  ui->spinBox_3->setEnabled(false);
}

PlayerSettingsDialog::~PlayerSettingsDialog() { delete ui; }

void PlayerSettingsDialog::on_buttonBox_accepted() {
  MainWindow *parent = qobject_cast<MainWindow *>(this->parentWidget());
  QPair<int, int> pos = parent->recording->grid.getVideoFilePositionById(
      parent->videoPlayer->at(playerIndex)->videoFileId);

  if (pos.first != ui->spinBox_2->value() ||
      pos.second != ui->spinBox_4->value())
    changesDone = true;

  if (changesDone) {
    // Apply changes to grid
    VideoFile vid = parent->recording->grid.getVideoFileById(
        parent->videoPlayer->at(playerIndex)->videoFileId);
    parent->recording->grid.deleteSource(pos.first, pos.second);
    if (parent->recording->grid
            .grid[ui->spinBox_2->value()][ui->spinBox_4->value()]
            .id != 0) {
      parent->recording->grid.deleteSource(ui->spinBox_2->value(),
                                           ui->spinBox_4->value());
    }
    parent->recording->grid.addSource(vid, ui->spinBox_2->value(),
                                      ui->spinBox_4->value());

    // Delete all existing players
    for (int i = parent->videoPlayer->length() - 1; i >= 0; i--)
      parent->videoPlayerDelete(i);

    // Print them again
    parent->loadPlayersFromRecording();

    QMessageBox msgBox;
    msgBox.setText("Änderungen wurden gespeichert");
    msgBox.exec();
  }
}
