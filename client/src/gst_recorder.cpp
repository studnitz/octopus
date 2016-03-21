#include "gst_recorder.h"

GstRecorder::GstRecorder(QObject *parent) : QObject(parent) { QGst::init(); }

GstRecorder::~GstRecorder() {}

QGst::BinPtr GstRecorder::createVideoSrcBin() {
  QGst::BinPtr videoBin;

  try {
    QGst::ElementFactoryPtr encoder = QGst::ElementFactory::find("omxh264enc");
    if (!encoder) {
      // if we don't have omx (when we're not on a RPI), use x264enc instead
      videoBin = QGst::Bin::fromDescription(
          "v4l2src device=/dev/video0 ! x264enc tune=zerolatency "
          "byte-stream=true ");
      qDebug() << "Using x264enc on device /dev/video0";
    } else {
      videoBin = QGst::Bin::fromDescription("v4l2src ! omxh264enc ");
      qDebug() << "Using omxh264enc";
    }

    return videoBin;

  } catch (const QGlib::Error &error) {
    qDebug() << "Failed to create video source bin:" << error;
    return QGst::BinPtr();
  }
}

QGst::BinPtr GstRecorder::createVideoMuxBin() {
  QGst::BinPtr videoMux;

  try {
    videoMux = QGst::Bin::fromDescription("h264parse ! mp4mux");
    qDebug() << "Created videoMuxer with h264parse ! mp4mux";

    return videoMux;
  } catch (const QGlib::Error &error) {
    qDebug() << "Failed to created videoMux bin:" << error;
    return QGst::BinPtr();
  }
}

void GstRecorder::recordLocally() {
  QGst::BinPtr videoSrcBin = createVideoSrcBin();
  QGst::BinPtr videoMuxBin = createVideoMuxBin();
  QGst::ElementPtr sink = QGst::ElementFactory::make("filesink");

  QString currentTime =
      QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

  QString filename =
      QDir::currentPath() + QDir::separator() + currentTime + ".mp4";
  qDebug() << "writing to:" << filename;
  sink->setProperty("location", filename);

  if (!videoSrcBin || !sink) {
    qDebug() << "Error. One or more elements could not be created.";
    return;
  }

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(videoSrcBin, videoMuxBin, sink);

  videoSrcBin->link(videoMuxBin);
  videoMuxBin->link(sink);

  m_pipeline->bus()->addSignalWatch();
  QGlib::connect(m_pipeline->bus(), "message", this,
                 &GstRecorder::onBusMessage);

  m_pipeline->setState(QGst::StatePlaying);
}

void GstRecorder::createRtpSink(quint16 port, QString address) {
  QGst::BinPtr videoSrcBin = createVideoSrcBin();
  QGst::ElementPtr rtpbin = QGst::ElementFactory::make("rtpbin");
  QGst::ElementPtr h264pay = QGst::ElementFactory::make("rtph264pay");

  if (!videoSrcBin || !rtpbin) {
    qDebug() << "Error. One or more elements could not be created.";
    return;
  }

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(videoSrcBin, h264pay);
  videoSrcBin->link(h264pay);

  m_pipeline->add(rtpbin);
  // send_rtp_sink_0 is needed as a parameter for rtpbin for the configuration
  h264pay->link(rtpbin, "send_rtp_sink_0");

  QGst::ElementPtr RtpUdpSink = QGst::ElementFactory::make("udpsink");
  RtpUdpSink->setProperty("port", (int)port);
  RtpUdpSink->setProperty("host", address);
  if (!RtpUdpSink) {
    qFatal("Failed to create udpsink. Aborting...");
  }
  m_pipeline->add(RtpUdpSink);
  rtpbin->link("send_rtp_src_0", RtpUdpSink);

  QGst::ElementPtr RtcpUdpSink = QGst::ElementFactory::make("udpsink");
  RtcpUdpSink->setProperty("port", port + 1);
  RtcpUdpSink->setProperty("host", address);
  RtcpUdpSink->setProperty("sync", false);  // needed for real-time
  RtcpUdpSink->setProperty("async", false);
  m_pipeline->add(RtcpUdpSink);
  rtpbin->link("send_rtcp_src_0", RtcpUdpSink);

  QGst::ElementPtr RtcpUdpSrc = QGst::ElementFactory::make("udpsrc");
  RtcpUdpSrc->setProperty("port", port + 2);
  m_pipeline->add(RtcpUdpSrc);
  RtcpUdpSrc->link(rtpbin, "recv_rtcp_sink_0");

  // watch the bus
  m_pipeline->bus()->addSignalWatch();
  QGlib::connect(m_pipeline->bus(), "message", this,
                 &GstRecorder::onBusMessage);

  qDebug() << "Streaming to RTP";

  m_pipeline->setState(QGst::StatePlaying);
}

void GstRecorder::stopRecording() {
  if (m_pipeline) m_pipeline->sendEvent(QGst::EosEvent::create());
}

void GstRecorder::stop() {
  m_pipeline->setState(QGst::StateNull);

  m_pipeline.clear();
}

void GstRecorder::onBusMessage(const QGst::MessagePtr &message) {
  switch (message->type()) {
    case QGst::MessageEos:
      // got end-of-stream - stop the pipeline
      stop();
      break;
    case QGst::MessageError:
      // check if the pipeline exists before destroying it,
      // as we might get multiple error messages
      if (m_pipeline) {
        stop();
      }
      qDebug() << message.staticCast<QGst::ErrorMessage>()->error().message();
      break;
    default:
      break;
  }
}
