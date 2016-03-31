#ifndef GSTEXPORTER_H
#define GSTEXPORTER_H

#include "recording.h"

#include <QObject>
#include <QGlib/Error>
#include <QGlib/Connect>
#include <QGst/Init>
#include <QGst/ElementFactory>
#include <QGst/Pipeline>
#include <QGst/Bus>
#include <QGst/Pad>
#include <QGst/Event>
#include <QGst/Query>
#include <QGst/Message>
#include <QGst/GhostPad>

#include <QTimer>
/**
 * @brief The GstExporter class is used to export a Recording (in a grid) with
 * many unique videofiles into a singular file. This video is fed with the many
 * subvideos and has the same gridlayout as the initial Recording. This is done
 * by using a dynamic GStreamer pipeline, which reads from a Recording object.
 */
class GstExporter : public QObject {
  Q_OBJECT
 public:
  /**
   * @brief GstExporter constructs a exporter with those settings
   * @param rec the Recording object with its grid and metadata
   * @param widthPx the desired width of the exported video in Pixel
   * @param heightPx the desired height of the exported video in Pixel
   * @param parent the parent object
   */
  explicit GstExporter(Recording* rec, quint16 widthPx, quint16 heightPx,
                       QObject* parent = 0);
  ~GstExporter();

 signals:

  /**
   * @brief progressChanged signal emitted when export progress has changed
   * @param value the export progress in percent
   */
  void progressChanged(float value);

  /**
   * @brief exportFinished signal when an export has finished
   * @param withoutError whether it finished without errors
   */
  void exportFinished(bool withoutError);

 public slots:

  /**
   * @brief exportVideo starts exporting the video by starting the Gstreamer
   * pipeline
   */
  void exportVideo();

  /**
   * @brief progressPercent queries the current progress in the exporting
   * pipeline and updates the progress variable
   */
  void progressPercent();

 private:
  /**
   * @brief usesOmx whether to use OpenMAX or not, as OpenMAX is needed by the
   * RaspberryPI
   */
  bool usesOmx = false;

  /**
   * @brief rec_ the Recording object we use to get the informations about our
   * video
   */
  Recording* rec_;

  /**
   * @brief rowCount the row count of the grid in the recording
   */
  quint16 rowCount;

  /**
   * @brief columnCount the column count of the grid in the recording
   */
  quint16 columnCount;

  /**
   * @brief exportHeightPx the desired height of the exported video
   */
  quint16 exportHeightPx;
  /**
   * @brief exportWidthPx the desired width of the exported video
   */
  quint16 exportWidthPx;

  /**
   * @brief elementHeightPx the calculated height of a grid element in the
   * export
   */
  quint16 elementHeightPx;
  /**
   * @brief elementWidthPx the calculated width of a grid elment in the export
   */
  quint16 elementWidthPx;

  /**
   * @brief m_pipeline the Gstreamer pipeline we use
   */
  QGst::PipelinePtr m_pipeline;

  /**
   * @brief timer timer used for updating the progress of the export
   */
  QTimer* timer;

  /**
   * @brief onBusMessage is called when we get new messages from the pipeline
   * bus
   * @param message the Gstreamer message
   */
  void onBusMessage(const QGst::MessagePtr& message);

  /**
   * @brief createFileSrcBin createsa QGst::Bin with our filesrc and decoders
   * and demuxers
   * @param path the path to the videofile
   * @param i internal counter variable to identify later the bins
   * @return the filesource bin
   */
  QGst::BinPtr createFileSrcBin(const QString path, const int i);

  /**
   * @brief createVideoMixer creates our videomixer with the appropiate sinks
   * extracted from the Recording
   * @return the video mixer bin
   */
  QGst::BinPtr createVideoMixer();

  /**
   * @brief stop stops the pipeline. Usually called when exporting is finished
   * @param withoutError whether it was stopped because of an error or not
   */
  void stop(bool withoutError = true);

  /**
   * @brief createEncoder creates the encoder (as it depends on the used
   * platform) and the parse elements in a bin
   * @return the encoder bin with the encoder, parser and muxer
   */
  QGst::BinPtr createEncoder();

  /**
   * @brief createDecoder creates the encoder for the source files (again it
   * depends on the platform used)
   * @param i count variable to uniquely identify the bin
   * @return the decoder bin with the decoder and the parser
   */
  QGst::BinPtr createDecoder(const int i);

  /**
   * @brief callbackNewPad is a called when the demuxer of sourcefile adds a
   * pad. It links the demuxer video pad to the appropiate vidermixer sink pad
   * @param sender the sending demuxer
   * @param pad the added pad (usually a video pad)
   */
  void callbackNewPad(const QGst::ElementPtr& sender, const QGst::PadPtr& pad);

  /**
   * @brief createCapsFilter is a convenience method to easily create video caps
   * @param width the desired width for the caps
   * @param height the desired height for the caps
   * @return the capsfilter with the desired caps
   */
  QGst::ElementPtr createCapsFilter(const quint16 width, const quint16 height);

  /**
   * @brief nameWithIndex is a convenience method to create a name that can be
   * used for naming Gstreamer elements (as they only char* for names)
   * @param name the desired name
   * @param i the id
   * @return the concatenated char* "name + i"
   */
  char* nameWithIndex(const QString name, const quint16 i);
};

#endif  // GSTEXPORTER_H
