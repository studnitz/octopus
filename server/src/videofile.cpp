#include "videofile.h"

VideoFile::VideoFile(quint32 id) : id(id) {}

VideoFile::VideoFile(quint32 id, bool isRemote, QString filePath,
                     QString hostname, qint64 deltaMs)
    : id(id),
      isRemote(isRemote),
      filePath(filePath),
      hostname(hostname),
      deltaMs(deltaMs) {}
