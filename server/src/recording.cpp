#include "recording.h"
#include <QFile>
#include <QJsonDocument>
Recording::Recording(QObject *parent) : QObject(parent) {}

Recording::Recording(QDateTime datetime, Grid grid)
  : datetime(datetime), grid(grid) {}

void Recording::write(QJsonObject &json) const
{
  json["datetime"] = datetime.toString();
  QJsonObject gridObject;
  grid.write(gridObject);
  json["grid"] = gridObject;
}

bool Recording::saveRecording() const {
  QFile saveFile("save.json");

  if (!saveFile.open(QIODevice::WriteOnly)) {
      qWarning("Couldn't open save file.");
      return false;
    }

  QJsonObject recordingObject;
  write(recordingObject);
  QJsonDocument saveDoc(recordingObject);
  saveFile.write(saveDoc.toJson());

 // qDebug() << "saved recording!";

  return true;
}
