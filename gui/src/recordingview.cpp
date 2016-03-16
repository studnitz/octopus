#include "recordingview.h"
#include "mainwindow.h"
#include <QGridLayout>
#include <QLabel>

RecordingView::RecordingView(QObject *parent, QWidget *tab) : QObject(parent) {
  this->tab = tab;
  // updateGrid();
  // updateVideoDevices();
}

void RecordingView::record_button(QPushButton *recordButton) {
  static bool startStopFlag = false;
  if (startStopFlag == false) {
    recordButton->setText("Stop");
    updateVideoDevices();
    updateGrid();
    recordStart();
    startStopFlag = true;
  } else {
    recordButton->setText("Aufnehmen");
    recordStop();
    startStopFlag = false;
  }
}

void RecordingView::updateGrid() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QGridLayout *gridLayout = tab->findChild<QGridLayout *>("recordingGrid");
  quint16 dimension = (quint16)ceil(sqrt(videoDeviceList->count()));
  for (int j = 0; j < dimension; ++j) {
    quint16 count = 0;
    for (int k = 0; k < dimension; ++k) {
      // Out-of-Bounds checking
      if (count >= videoDeviceList->count()) {
        break;
      } else {
        gridLayout->addWidget(new QLabel(videoDeviceList->item(count)->text()), j, k);
        count++;
      }
    }
    if (count >= videoDeviceList->count()) { break; }
  }
}

void RecordingView::updateVideoDevices() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  videoDeviceList = tab->findChild<QListWidget *>("deviceListWidget");
  videoDeviceList->clear();
  QList<ClientGui *> *clientList = p->guiInterface->clients;
  for (int i = 0; i < clientList->length(); ++i) {
    ClientGui *client = clientList->at(i);
    foreach (const QString &deviceName, client->devices) {
      videoDeviceList->addItem(client->name + ": " + deviceName);
    }
  }
}

void RecordingView::recordStart() {
  /* magic */
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  p->log("Aufnahme starten");
}

void RecordingView::recordStop() {
  /* magic */
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  p->log("Aufnahme stoppen");
}
