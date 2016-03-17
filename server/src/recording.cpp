#include "recording.h"
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QDebug>
Recording::Recording() : datetime(QDateTime::currentDateTime()), grid(Grid()) {}

Recording::Recording(QDateTime datetime, Grid &grid)
    : datetime(datetime), grid(grid) {}

void Recording::read(const QJsonObject &json) {
  datetime = QDateTime::fromString(json["datetime"].toString());
  grid.read(json["grid"].toObject());
  qDebug() << QJsonDocument(json).toJson(QJsonDocument::Compact);
}

void Recording::write(QJsonObject &json) const {
  json["datetime"] = datetime.toString();
  QJsonObject gridObject;
  grid.write(gridObject);
  qDebug() << "WRITEEEEEEEEEEE";
  json["grid"] = gridObject;
}

bool Recording::saveRecording() const {
  QString recordingTime = datetime.toString("yyyy_MM_dd_hh_mm_ss");
  QDir saveDir = QDir::current();
  saveDir.mkdir("recordings");
  saveDir.cd("recordings");
  QFile saveFile(saveDir.absoluteFilePath(recordingTime.append(".off")));
  qDebug() << saveDir.absolutePath();
  if (!saveFile.open(QIODevice::WriteOnly)) {
    qWarning("Couldn't open save file.");
    return false;
  }

  QJsonObject recordingObject;
  qDebug() << "REKT";
  write(recordingObject);
  qDebug() << "RECOBJECTT" << recordingObject;
  QJsonDocument saveDoc(recordingObject);
  saveFile.write(saveDoc.toJson());

  qDebug() << "saved recording!";

  return true;
}

bool Recording::loadRecording(QString path) {
  QFile loadFile(path);

  if (!loadFile.open(QIODevice::ReadOnly)) {
    qWarning("Couldn't open recording file.");
    return false;
  }
  QByteArray recordingData = loadFile.readAll();

  QJsonDocument loadDoc(QJsonDocument::fromJson(recordingData));

  read(loadDoc.object());

  return true;
}
