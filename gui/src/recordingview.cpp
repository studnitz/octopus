#include "recordingview.h"
#include "mainwindow.h"

RecordingView::RecordingView(QObject *parent, QWidget *tab) : QObject(parent) {
  this->tab = tab;
  recGrid = new RecordingGrid();
  videoDeviceListModel = new QStringListModel();
  videoDeviceListView = tab->findChild<QListView *>("videoDeviceListView");
  videoDeviceListView->setModel(videoDeviceListModel);
}

void RecordingView::record_button(QPushButton *recordButton) {
  recordStart();
  createAndStartRecording();
}

void RecordingView::createAndStartRecording() {
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
  quint16 resolution = p->settings->value("octopus/Quality").toInt();
  quint16 fps, height, width;
  fps = p->settings->value("octopus/Fps").toInt();

  switch (resolution) {
    case 0: {
      width = 320;
      height = 240;
      break;
    }
    case 1:
      width = 640;
      height = 480;
      break;
    case 2:
      width = 1280;
      height = 960;
      break;
    default:
      width = 640;
      height = 480;
      break;
  }

  data1["width"] = width;
  data1["height"] = height;
  data1["fps"] = fps;
  data["data"] = data1;
  qDebug() << "JSONREC:" << data;
  p->guiInterface->sendData("recordLocally", data);
}

void RecordingView::updateVideoDeviceList(QStringList list) {
  qDebug() << list;
  videoDeviceListModel->setStringList(list);
}

void RecordingView::updateGrid() {
  gridLayout = tab->findChild<QGridLayout *>("recordingGrid");
  dimension = (quint16)ceil(sqrt(videoDeviceListModel->rowCount()));
  quint16 count = 0;
  for (int j = 0; j < dimension; ++j) {
    for (int k = 0; k < dimension; ++k) {
      // Out-of-Bounds checking
      if (count >= videoDeviceListModel->rowCount()) {
        QLabel *emptyLabel = new QLabel("empty");
        emptyLabel->setFrameStyle(QFrame::Panel);
        emptyLabel->setAlignment(Qt::AlignCenter);
        QFont currentFont = emptyLabel->font();
        currentFont.setPointSize(24);
        currentFont.setItalic(true);
        emptyLabel->setFont(currentFont);
        gridLayout->addWidget(emptyLabel, j, k);
      } else {
        QLabel *frameLabel =
            new QLabel(videoDeviceListModel->stringList().at(count));
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

void RecordingView::updateGrid2() {
  gridLayout = tab->findChild<QGridLayout *>("recordingGrid");
  for (int i = 0; i < recGrid->height(); ++i) {
    for (int j = 0; j < recGrid->width(); ++j) {
    }
  }
}

QLabel *RecordingView::createEmptyEntry() const {
  QLabel *result = new QLabel("empty");
  result->setFrameStyle(QFrame::Panel);
  result->setAlignment(Qt::AlignCenter);
  QFont currentFont = result->font();
  currentFont.setPointSize(24);
  currentFont.setItalic(true);
  result->setFont(currentFont);

  return result;
}

QLabel *RecordingView::createEntry(RecordingGrid::GridElement *entry) const {
  QString labeltext = entry->hostname + ": " + entry->device;
  QLabel *result = new QLabel(labeltext);
  result->setFrameStyle(QFrame::Panel);
  result->setAlignment(Qt::AlignCenter);
  QFont currentFont = result->font();
  currentFont.setPointSize(24);
  result->setFont(currentFont);

  return result;
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
