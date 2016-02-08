#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QObject>
#include <QFile>
#include <QJsonObject>

class VideoFile {
 public:
  VideoFile(quint32 id = 0);  // empty Video-File
  VideoFile(quint32 id, bool isRemote, QString filepath, QString hostname,
            qint64 deltaMs = 0);
  void read(const QJsonObject &json);
  void write(QJsonObject &json) const;

  quint32 id;
  bool isRemote;
  QString filepath;
  QString hostname;
  qint64 deltaMs;
};

#endif  // VIDEOFILE_H
