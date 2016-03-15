#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QObject>
#include <QFile>
#include <QJsonObject>

class VideoFile {
 public:
  /**
   * @brief The Orientation enum describes in which orientation a video is shown
   * in the grid. Up is 0°, Right 90°, Down 180° and Left 270° rotation.
   */
  enum Orientation { Up, Right, Down, Left };
  /**
   * @brief VideoFile Represents a live or recorded video. This constructor
   * creates an empty Videofile.
   * @param id Unique ID to identify a videofile.
   */
  VideoFile(quint32 id = 0);  // empty Video-File

  /**
   * @brief VideoFile Represents a live or recorded video.
   * @param id Unique ID to identify a videofile.
   * @param isRemote flag whether the video is on a remote client or on
   * localhost.
   * @param filepath location of the video. Can also be a live source like
   * /dev/video0/
   * @param hostname the hostname on which the video file is on
   * @param orientation Used orientation for the grid layout.
   * @param deltaMs offset in playback to the base Recording.
   */
  VideoFile(quint32 id, bool isRemote, QString& filepath, QString& hostname,
            VideoFile::Orientation orientation = Up, qint64 deltaMs = 0);

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

  quint32 id;
  bool isRemote;
  QString filepath;
  QString hostname;
  Orientation orientation;
  qint64 deltaMs;
};

#endif  // VIDEOFILE_H
