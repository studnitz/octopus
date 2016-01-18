#include "videoplayer.h"
#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>

VideoPlayer::VideoPlayer(QWidget *parent, quint8 index) : QVideoWidget(parent) {
  this->index = index;
}

void VideoPlayer::contextMenuEvent(QContextMenuEvent *e) {

  QMenu myContext;
  myContext.addAction("Einstellungen");
  myContext.addAction("Player löschen");
  QAction *selectedItem = myContext.exec(e->globalPos());

  if (selectedItem) {
    if (QString::compare(selectedItem->text(), "Player löschen") == 0) {
      QMessageBox msgBox;
      msgBox.setWindowTitle("Player löschen");
      msgBox.setInformativeText("Sind sie sich sicher?");
      msgBox.setText(QString("Sie sind dabei den Player mit der ID=")
                         .append(QString::number(this->index))
                         .append(" zu löschen."));
      msgBox.setStandardButtons(QMessageBox::Apply | QMessageBox::Abort);
      msgBox.setDefaultButton(QMessageBox::Apply);

      if (msgBox.exec() == QMessageBox::Apply) {
        emit playerDelete(this->index);
      }
    } else if (QString::compare(selectedItem->text(), "Einstellungen") == 0) {
        emit playerOpenOptions(this->index);
      }
    }
}
