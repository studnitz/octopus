#include "grid.h"

Grid::Grid(quint8 width, quint8 height) : width(width), height(height) {
  grid = QList<QList<VideoFile> >();
  for (int i = 0; i < height; i++) {
    QList<VideoFile> row = QList<VideoFile>();
    for (int j = 0; j < width; j++) {
      row.push_back(VideoFile());
    }
    grid.push_back(row);
  }
}

void Grid::addSource(VideoFile file, quint8 x, quint8 y) { grid[x][y] = file; }

void Grid::deleteSource(quint8 x, quint8 y) { grid[x][y] = VideoFile(); }

QPair<int, int> Grid::getVideoFilePositionById(quint32 id) {
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      if (grid[i][j].id == id) return QPair<int, int>(i, j);

  return QPair<int, int>(-1, -1);
}
