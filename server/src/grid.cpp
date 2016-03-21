#include "grid.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
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

void Grid::addSource(VideoFile& file, quint8 x, quint8 y) { grid[x][y] = file; }

void Grid::deleteSource(quint8 x, quint8 y) { grid[x][y] = VideoFile(); }

void Grid::resizeGrid(quint8 width, quint8 height)
{
  this->width = width;
  this->height = height;
}

void Grid::read(const QJsonObject &json) {
  grid.clear();
  grid = QList<QList<VideoFile> >();
  QJsonArray gridArray = json["grid"].toArray();
  for (int rowIndex = 0; rowIndex < gridArray.size(); ++rowIndex) {
    QList<VideoFile> row = QList<VideoFile>();
    QJsonArray rowArray = gridArray[rowIndex].toArray();
    for (int columnIndex = 0; columnIndex < rowArray.size(); ++columnIndex) {
      QJsonObject videoObject = rowArray[columnIndex].toObject();
      VideoFile video;
      video.read(videoObject);
      row.push_back(video);
    }
    grid.push_back(row);
  }
}

void Grid::write(QJsonObject &json) const {
  QJsonArray gridArray;
  foreach (const QList<VideoFile> row, grid) {
    QJsonArray rowArray;
    foreach (const VideoFile video, row) {
      QJsonObject videoObject;
      video.write(videoObject);
      rowArray.append(videoObject);
    }
    gridArray.append(rowArray);
  }
  json["grid"] = gridArray;
}

QPair<int, int> Grid::getVideoFilePositionById(quint32 id) {
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      if (grid[i][j].id == id) return QPair<int, int>(i, j);

  return QPair<int, int>(-1, -1);
}

VideoFile& Grid::getVideoFileById(quint32 id) {
  for (int i = 0; i < width; ++i)
    for (int j = 0; j < height; ++j)
      if (grid[i][j].id == id) return grid[i][j];

  VideoFile vid1 = VideoFile(0);
  return vid1;
}
