#include "videofile.h"

VideoFile::VideoFile()
{

}

VideoFile::VideoFile(quint32 id, bool isRemote, QFile videoFile, QString hostname, qint64 deltaMs)
{
  this->id = id;
  this->isRemote = isRemote;
  this->videoFile = videoFile;
  this->hostname = hostname;
  this->deltaMs = deltaMs;
}
