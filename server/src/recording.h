#ifndef RECORDING_H
#define RECORDING_H

#include "grid.h"
#include <QObject>
#include <QDateTime>

class Recording : public QObject {
 public:
  Recording();
  /**
   * @brief Recording Represents a recording with date and time and a grid.
   * @param datetime The current date and time of the start of the recording
   * @param grid The grid of VideoFile objects.
   */
  Recording(QDateTime datetime, Grid& grid);
  QDateTime datetime;
  Grid grid;

  /**
   * @brief read Serialization helper class, reads from a JSON object and
   * changes the variables accordingly.
   * @param json The JsonObject to read.
   */
  void read(const QJsonObject &json);
  /**
   * @brief write Serialization helper class, writes to JSON object accordingly
   * to the current parameters.
   * @param json The JsonObject to write.
   */
  void write(QJsonObject &json) const;

  /**
   * @brief saveRecording saves the recording to a file in the format
   * yyyy_MM_dd_hh_mm_ss.off, so it can be loaded again later
   * @return true if the save-file was written successfully, otherwise false.
   */
  bool saveRecording() const;
  /**
   * @brief loadRecording loads a recording on the given path.
   * @return true if the file was loaded successfully, otherwise false.
   */
  bool loadRecording(QString path);
};

#endif  // RECORDING_H
