#ifndef GST_RECORDER_H
#define GST_RECORDER_H

#include <QObject>
#include <QGlib/Error>
#include <QGlib/Connect>
#include <QGst/Init>
#include <QGst/ElementFactory>
#include <QGst/ChildProxy>
#include <QGst/Pipeline>
#include <QGst/Pad>
#include <QGst/Event>
#include <QGst/Message>
#include <QGst/Bus>
#include <QDir>

class GstRecorder : public QObject {
  Q_OBJECT
public:
  GstRecorder(QObject *parent = 0);

  /**
   * @brief recordLocally records locally to disk using x264enc
   * Starts recording locally to the current directory of the client using h264
   * encoding, provided by the gstreamer plugins omxh264enc (Raspberry Pi) or
   * x264 (rest).
   * The output file is named after the current date and time.
   * Format: yy_MM_dd_hh_mm_ss.mkv
   */
  void recordLocally();
  /**
   * @brief stop stops a ongoing recording or socket
   */
  void stop();
  /**
   * @brief createUdpSink creates an RTP sink for others to consume. Creates 3
   * ports.
   * @param port The port on which the UDP Video sink is created. Note that this
   * implicitly creates a RTCP control sink and source, on the ports "port + 1"
   * and "port + 2"
   * @param host The host on which the sinks are created. Defaults to 127.0.0.1
   */
  void createRtpSink(quint16 port = 1337, QString address = "127.0.0.1");

private:
  /**
   * @brief createVideoSrcBin is used to create a video source (usually v4l2)
   * @return a pointer to the created video source
   */
  QGst::BinPtr createVideoSrcBin();

  /**
   * @brief onBusMessage Event handler for bus messages
   * @param message the current bus message
   */
  void onBusMessage(const QGst::MessagePtr &message);

  /**
   * @brief m_pipeline the pipeline we use to create a recodring or a stream
   */
  QGst::PipelinePtr m_pipeline;
  /**
   * @brief createVideoMuxBin is used to create a videoMuxer, important for
   * local recording
   * @return a pointer to the created video muxer
   */
  QGst::BinPtr createVideoMuxBin();
};

#endif // GST_RECORDER_h
