#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <QJsonObject>

/**
 * @brief The VideoFile class represents a simple video-file - may it be inside
 * of a container or not - inside the grid. Every VideoFile should have a unique
 * id (except for the case that the VideoFile is empty). Read and write are
 * methods for conversion to JSON. Orientation is currently not used yet.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class VideoFile {
 public:

  /**
   * @brief The Orientation enum describes in which orientation a video is shown
   * in the grid. Up is 0°, Right 90°, Down 180° and Left 270° rotation.
   */
  enum Orientation { Up, Right, Down, Left };

  /**
   * @brief VideoFile Represents a live or recorded video. This constructor
   * creates an empty Videofile. Empty VideoFiles should have id = 0.
   * @param id unique ID to identify a videofile.
   */
  VideoFile(quint32 id = 0);

  /**
   * @brief VideoFile Represents a live or recorded video.
   * @param id Unique ID to identify a videofile
   * @param isRemote flag whether the video is on a remote client or on
   * localhost
   * @param filepath location of the video. Can also be a live source like
   * /dev/video0/
   * @param hostname the hostname on which the video file is on
   * @param orientation used for potential turning of the videos in the grid
   * layout
   * @param deltaMs offset in playback to the base Recording.
   */
  VideoFile(quint32 id, bool isRemote, QString& filepath, QString& hostname,
            VideoFile::Orientation orientation = Up, qint64 deltaMs = 0);

  /**
   * @brief read Serialization helper class, reads from a JSON object and
   * changes the variables accordingly. read is to be called on an empty object,
   * so the empty object can be filled with the data from the JSON-file.
   * @param json The JsonObject to read from
   */
  void read(const QJsonObject& json);

  /**
   * @brief write Serialization helper class, writes to JSON object accordingly
   * to the current parameters. json should be an empty QJsonObject to be
   * written to.
   * @param json The JsonObject to write to
   */
  void write(QJsonObject& json) const;

  /**
   * @brief id is a unique ID to identify a videofile.
   */
  quint32 id;

  /**
   * @brief isRemote is a flag whether the video is on a remote client or on.
   */
  bool isRemote;

  /**
   * @brief filepath stores the location of the video. Can also be a live source
   * like
   * /dev/video0/.
   */
  QString filepath;

  /**
   * @brief hostname is the name of the host that stores the video file.
   */
  QString hostname;

  /**
   * @brief orientation can be used to save an eventual rotation of the videos
   * in the grid layout.
   */
  Orientation orientation;

  /**
   * @brief deltaMs saves the offset of this one VideoFile to make all
   * VideoFiles synchronous in playback.
   */
  qint64 deltaMs;
};

#endif  // VIDEOFILE_H
