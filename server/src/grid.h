#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QPair>
#include "videofile.h"

class Grid {
 public:
  /**
   * @brief Grid Represents a grid of video sources.
   * @param width The width of the grid.
   * @param height The height of the grid.
   */
  Grid(quint8 width = 3, quint8 height = 3);

  /**
   * @brief addSource adds a video source to the grid.
   * @param file The VideoFile to add.
   * @param x The x-coordinate of the VideoFile in the grid.
   * @param y The y-coordinate of the VideoFile in the grid
   */
  void addSource(VideoFile& file, quint8 x, quint8 y);
  /**
   * @brief deleteSource deletes the videosource in the grid on the given
   * position
   */
  void deleteSource(quint8 x, quint8 y);

  /**
   * @brief resizeGrid resizes the grid to the new dimenions.
   */
  void resizeGrid(quint8 width, quint8 height);

  /**
   * @brief read Serialization helper class, reads from a JSON object and
   * changes the variables accordingly.
   * @param json The JsonObject to read.
   */
  void read(const QJsonObject& json);
  /**
   * @brief write Serialization helper class, writes to JSON object accordingly
   * to the current parameters.
   * @param json The JsonObject to write.
   */
  void write(QJsonObject& json) const;

  QPair<int, int> getVideoFilePositionById(quint32 id);
  VideoFile& getVideoFileById(quint32 id);
  QList<QList<VideoFile> > grid;
  quint8 width;
  quint8 height;
};

#endif  // GRID_H
