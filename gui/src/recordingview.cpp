#include "recordingview.h"
#include "mainwindow.h"
#include <QLabel>

RecordingView::RecordingView(QObject *parent, QWidget *tab) : QObject(parent) {
  this->tab = tab;
}

void RecordingView::record_button(QPushButton *recordButton) {
  static bool startStopFlag = false;
  if (startStopFlag == false) {
    recordButton->setText("Stop");
    createRecording();
    recordStart();
    startStopFlag = true;
  } else {
    recordButton->setText("Aufnehmen");
    recordStop();
    startStopFlag = false;
  }
}

void RecordingView::createRecording() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  Grid grid = Grid(gridLayout->columnCount(), gridLayout->rowCount());
  quint32 id = 1;

  for (int i = 0; i < gridLayout->rowCount(); ++i) {
    for (int j = 0; j < gridLayout->columnCount(); ++j) {
      QLabel *currentItem =
          (QLabel *)gridLayout->itemAtPosition(i, j)->widget();
      QString current = currentItem->text();

      if (current == "empty") break;  // skip empty GridFrames
      // Current Videodevice is in format "hostname: /dev/videoX"
      QStringList videoInfo = current.split(": ");
      QString hostname = videoInfo.at(0);

      QString devicepath = videoInfo.at(1);
      VideoFile vid = VideoFile(id, true, "", hostname, devicepath);
      grid.addSource(vid, j, i);
      id++;
    }
  }
  rec = new Recording(QDateTime::currentDateTime(), grid);
  rec->saveRecording();
  QJsonObject data1, data;
  rec->write(data1);
  data["data"] = data1;
  qDebug() << "JSONREC:" << data;
  p->guiInterface->sendData("recordLocally", data);
}

void RecordingView::updateGrid() {
  gridLayout = tab->findChild<QGridLayout *>("recordingGrid");
  dimension = (quint16)ceil(sqrt(videoDeviceList->count()));
  quint16 count = 0;
  for (int j = 0; j < dimension; ++j) {
    for (int k = 0; k < dimension; ++k) {
      // Out-of-Bounds checking
      if (count >= videoDeviceList->count()) {
        QLabel *emptyLabel = new QLabel("empty");
        emptyLabel->setFrameStyle(QFrame::Panel);
        emptyLabel->setAlignment(Qt::AlignCenter);
        QFont currentFont = emptyLabel->font();
        currentFont.setPointSize(24);
        currentFont.setItalic(true);
        emptyLabel->setFont(currentFont);
        gridLayout->addWidget(emptyLabel, j, k);
      } else {
        QLabel *frameLabel = new QLabel(videoDeviceList->item(count)->text());
        frameLabel->setFrameStyle(QFrame::Panel);
        frameLabel->setAlignment(Qt::AlignCenter);
        QFont currentFont = frameLabel->font();
        currentFont.setPointSize(24);
        frameLabel->setFont(currentFont);
        gridLayout->addWidget(frameLabel, j, k);
      }
      count++;
    }
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
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QJsonObject data;
  // p->guiInterface->sendData("recordLocally", data);
  p->log("Aufnahme starten");
}

void RecordingView::recordStop() {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());
  QJsonObject data;
  p->guiInterface->sendData("stopCameras", data);
  p->log("Aufnahme stoppen");
}
