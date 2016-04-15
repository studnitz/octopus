#ifndef GRID_H
#define GRID_H

#include <QObject>
#include <QPair>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

#include "videofile.h"

/**
 * @brief The Grid class represents a grid, consisting of a 2-dimensional QList
 * storing VideoFile objects, methods to add and delete from the grid, methods
 * for search in the grid and the height and width-attributes.
 * @author Bartosz Milejski, Yannick Schädle, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class Grid {
 public:
  /**
   * @brief Constructor for a grid. Pretty straightforward.
   * @param width The width of the grid.
   * @param height The height of the grid.
   */
  Grid(quint8 width = 3, quint8 height = 3);

  /**
   * @brief addSource adds a video source to the grid. Note that VideoFile is
   * passed on call-by-reference.
   * @param file A reference of the VideoFile to add
   * @param x The x-coordinate of the VideoFile in the grid
   * @param y The y-coordinate of the VideoFile in the grid
   */
  void addSource(VideoFile& file, quint8 x, quint8 y);

  /**
   * @brief deleteSource deletes the VideoFile in the grid on the given
   * position.
   * @param x The x-coordinate of the VideoFile in the grid
   * @param y The y-coordinate of the VideoFile in the grid
   */
  void deleteSource(quint8 x, quint8 y);

  /**
   * @brief resizeGrid changes the width and height attributes of the grid,
   * effectively allowing one additional row and col in the 2-dimensional QList
   * of VideoFiles
   * @param width The new width of the grid
   * @param height The new height of the grid
   */
  void resizeGrid(quint8 width, quint8 height);

  /**
   * @brief Serialization helper method, reads from a JSON object and
   * changes the variables accordingly. read is to be called on an empty object,
   * so the empty object can be filled with the data from the JSON-file.
   * @param json The JsonObject to read from
   */
  void read(const QJsonObject& json);

  /**
   * @brief Serialization helper method, writes to a JSON object accordingly
   * to the current parameters. json should be an empty QJsonObject to be
   * written to.
   * @param json The JsonObject to write to
   */
  void write(QJsonObject& json) const;

  /**
   * @brief getVideoFilePositionById loops through the 2-dimensional QList of
   * VideoFiles and returns the position where the videoFile with videoFile.id =
   * id is.
   * @param id The id to look up
   * @return a QPair of ints containing the position of the VideoFile if the id
   * is found, (-1, -1) otherwise
   */
  QPair<int, int> getVideoFilePositionById(quint32 id);

  /**
   * @brief getVideoFileById loops through the 2-dimensional QList of VideoFiles
   * and returns a reference to the videoFile where videoFile.id = id if id is
   * found. Use with caution at the moment. Unexpected behavior can occur if the
   * looked up VideoFile does not exist, since a reference to a locally created
   * stack-object is beeing the return value then. Of course the variable on the
   * stack is not kept infinitely and deallocated as soon as the corresponding
   * scope is left. So effectively the return value can point anywhere if id
   * doesn't exist.
   * @param id The id to be looked for
   * @return a reference to the VideoFile-object with the given id. Random value
   * if it doesn't
   */
  VideoFile& getVideoFileById(quint32 id);

  /**
   * @brief size the current amount of elements in the grid
   * @return the amount of elements
   */
  qint16 size();

  /**
   * @brief grid is a 2-dimensinal list of VideoFiles. Pretty straightforward.
   */
  QList<QList<VideoFile> > grid;

  /**
   * @brief width stores the width of the Grid. Should be identical to
   * grid.length().
   */
  quint8 width;

  /**
   * @brief height stores the height of the Grid. Should be identical to
   * grid.at(0).length(). However grid.at(0).length() can cause exceptions when
   * the Grid is empty.
   */
  quint8 height;
};

#endif  // GRID_H
