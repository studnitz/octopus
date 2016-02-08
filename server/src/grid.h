#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QPair>
#include "videofile.h"

class Grid {
 public:
  Grid(quint8 width = 3, quint8 height = 3);

  void addSource(VideoFile file, quint8 x, quint8 y);
  void deleteSource(quint8 x, quint8 y);

  void resizeGrid(quint8 x, quint8 y);
  QPair<int, int> getVideoFilePositionById(quint32 id);

  QList<QList<VideoFile> > grid;
 private:
  quint8 width;
  quint8 height;
};

#endif  // GRID_H
