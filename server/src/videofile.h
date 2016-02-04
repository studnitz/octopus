#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QObject>
#include <QFile>

class VideoFile {
 public:
  VideoFile(quint32 id = 0);  // empty Video-File
  VideoFile(quint32 id, bool isRemote, QFile *file, QString hostname,
            qint64 deltaMs = 0);

  quint32 id;
  bool isRemote;
  QFile *file;
  QString hostname;
  qint64 deltaMs;
};

#endif  // VIDEOFILE_H
