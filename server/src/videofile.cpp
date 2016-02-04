#include "videofile.h"

VideoFile::VideoFile(quint32 id) : id(id) {}

VideoFile::VideoFile(quint32 id, bool isRemote, QFile *file,
                     QString hostname, qint64 deltaMs)
    : id(id),
      isRemote(isRemote),
      file(file),
      hostname(hostname),
      deltaMs(deltaMs) {}
