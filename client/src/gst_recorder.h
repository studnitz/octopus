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
#include <QGst/GhostPad>
#include <QGst/Event>
#include <QGst/Message>
#include <QGst/Bus>
#include <QGst/Caps>
#include <QGst/Fraction>
#include <QHostAddress>
#include <QDir>

class GstRecorder : public QObject {
  Q_OBJECT
 public:
  GstRecorder(quint16 width = 640, quint16 height = 480, quint16 fps = 30, QString device = "/dev/video0",
              QObject *parent = 0);
  ~GstRecorder();

  /**
   * @brief recordLocally records locally to disk using x264enc
   * Starts recording locally to the current directory of the client using h264
   * encoding, provided by the gstreamer plugins omxh264enc (Raspberry Pi) or
   * x264 (rest).
   * The output file is named after the current date and time.
   * Format: yy_MM_dd_hh_mm_ss.mkv
   *
   * @bug We call it fondly the "Video-device busy"-bug. The good thing is, it's
   * not our fault. The bad thing is: That doesn't really matter.
   * The bug is the following: When starting and stopping recordings multiple
   * times in a row without restarting the client application, eventually a
   * message will show up telling you that the video-device is busy and that a
   * recording could not be started. A not-critital error message could already
   * show up on stopping the recording before that. The bug lies with gstreamer
   * or the v4l2 interface (bug number 732912 - should officially be fixed...)
   * We downloaded an official gstreamer example to try just the record start
   * and stop functionality with their GUI. On every device that we tried the
   * error was reproducable. However the tries it took to reproduce varied a
   * lot. Between 2 and 30 recordings were needed. No clear pattern recognizable
   * to me. Waiting for a gstreamer update seems best to me.
   */
  const QString recordLocally();

  /**
   * @brief stopRecording
   */
  void stopRecording();

  /**
   * @brief stop stops a ongoing recording or socket
   *
   * @bug We call it simply the Stop-Bug. It's not that bad. It refers to the
   * phenomenon that sometimes (we didn't find a pattern yet) the recording
   * won't stop. Or to be more precise, it stops, but immediately after that it
   * starts again. It's not critical because a file is created and can be used
   * normally. And the second recording can be killed by killing the client
   * application. Not nice though. Especially because you see that the webcam
   * light is still on after clicking on stop.
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

  quint16 videoWitdhPx;

  quint16 videoHeightPx;

  quint16 framerate;

  QString devicepath;

  quint16 deviceNumber;

  bool usesOmx = false;

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
  QGst::ElementPtr createCapsFilter(const quint16 width, const quint16 height, const quint16 fps);
  quint16 getDeviceNumber(QString path);
};

#endif  // GST_RECORDER_h
