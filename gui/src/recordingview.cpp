#include "recordingview.h"
#include "mainwindow.h"

RecordingView::RecordingView(QObject *parent, QWidget *tab) : QObject(parent) {
  this->tab = tab;
}

void RecordingView::record_button(QPushButton *recordButton) {
  static bool startStopFlag = false;
  if (startStopFlag == false) {
    recordButton->setText("Stop");
    recordStart();
    startStopFlag = true;
  } else {
    recordButton->setText("Aufnehmen");
    recordStop();
    startStopFlag = false;
  }
}

void RecordingView::recordStart() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QString data("");
  p->guiInterface->sendData("recordLocally", data);
  p->log("Aufnahme starten");
}

void RecordingView::recordStop() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QString data("");
  p->guiInterface->sendData("stopCameras", data);
  p->log("Aufnahme stoppen");
}
