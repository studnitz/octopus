#include "videofile.h"

#include <iostream>
#include <QDebug>

VideoFile::VideoFile(quint32 id)
    : id(id),
      isRemote(false),
      filepath(""),
      hostname(""),
      devicepath(""),
      orientation(Up),
      deltaMs(0) {}

VideoFile::VideoFile(quint32 id, bool isRemote, QString filepath,
                     QString hostname, QString devicepath,
                     Orientation orientation, qint64 deltaMs)
    : id(id),
      isRemote(isRemote),
      filepath(filepath),
      hostname(hostname),
      devicepath(devicepath),
      orientation(orientation),
      deltaMs(deltaMs) {}

void VideoFile::read(const QJsonObject &json) {
  qDebug() << "VIDEOREAD" << json;
  id = json["id"].toInt();
  isRemote = json["isRemote"].toBool();
  filepath = json["filepath"].toString();
  hostname = json["hostname"].toString();
  devicepath = json["devicepath"].toString();
  // static cast needed to convert int -> enum Orientation
  orientation = static_cast<Orientation>(json["orientation"].toInt());
  deltaMs = json["deltaMs"].toInt();
}

void VideoFile::write(QJsonObject &json) const {
  qDebug() << "VIDEOWRITE BEGGIN";
  qDebug() << "id: " << id;
  json["id"] = (double)id;
  qDebug() << "isRemote" << isRemote;
  json["isRemote"] = isRemote;
  qDebug() << "filepath" << filepath;
  json["filepath"] = filepath;
  qDebug() << "hostname" << hostname;
  json["hostname"] = hostname;
  qDebug() << "devicepath" << devicepath;
  json["devicepath"] = devicepath;
  qDebug() << "orintation" << orientation;
  json["orientation"] = orientation;
  qDebug() << "delta" << deltaMs;
  json["deltaMs"] = deltaMs;
  qDebug() << "VIDEOWRITE" << json;
}
