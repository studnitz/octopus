#ifndef GRID_H
#define GRID_H

#include <QObject>
#include "videofile.h"

class Grid {
 public:
  Grid(quint8 width = 3, quint8 height = 3);

  void addSource(VideoFile file, quint8 x, quint8 y);
  void deleteSource(quint8 x, quint8 y);

  void resizeGrid(quint8 x, quint8 y);

  void read(const QJsonObject &json);
  void write(QJsonObject &json) const;

 private:
  quint8 width;
  quint8 height;
  QList<QList<VideoFile> > grid;
};

#endif  // GRID_H
