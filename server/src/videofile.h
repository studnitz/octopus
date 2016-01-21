#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QObject>
#include <QFile>


class VideoFile
{
public:
  VideoFile(quint32 id, bool isRemote = false, QFile videoFile);
};

#endif // VIDEOFILE_H
