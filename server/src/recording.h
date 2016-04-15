#ifndef RECORDING_H
#define RECORDING_H

#include "grid.h"
#include <QDateTime>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QDebug>


/**
 * @brief The Recording class represents a complete recording. Every recording
 * has it's own grid since the camera constellation can be different every time
 * when recording. All the information from a recording can be written to and
 * read from any .off file quite easy (in JSON format) using the Recording::read
 * and Recording::write methods.
 * @author Bartosz Milejski, Yannick Schädle, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class Recording : public QObject {
 public:
  /**
   * @brief Standard-constructor for an empty Recording.
   */
  Recording();

  /**
   * @brief Constructs a Recording, using a QDateTime and a Grid. Note that the
   * grid parameter is passed call-by-reference.
   * @param datetime The date and the time of the start of the recording
   * @param grid A reference to a grid containing VideoFile objects.
   */
  Recording(QDateTime datetime, Grid &grid);

  /**
   * @brief datetime The date and the time of the start of the recording.
   */
  QDateTime datetime;

  /**
   * @brief grid contains a reference of the Grid-object containing all the
   * VideoFiles and their position and orientation.
   */
  Grid grid;

  /**
   * @brief Serialization helper method, reads from a JSON object and
   * changes the variables accordingly. read is to be called on an empty object,
   * so the empty object can be filled with the data from the JSON-file.
   * @param json The JsonObject to read from
   */
  void read(const QJsonObject &json);

  /**
   * @brief Serialization helper method, writes to JSON object accordingly
   * to the current parameters. json should be an empty QJsonObject to be
   * written to.
   * @param json The JsonObject to write to
   */
  void write(QJsonObject &json) const;

  /**
   * @brief saveRecording saves the recording to a file in the format
   * yyyy_MM_dd_hh_mm_ss.off, so it can be loaded again later.
   * @return true if the save-file was written successfully, otherwise false
   */
  bool saveRecording() const;

  /**
   * @brief loadRecording loads a recording from the given path.
   * @return true if the file was loaded successfully, otherwise false
   */
  bool loadRecording(QString path);
};

#endif  // RECORDING_H
