#include "gst_recorder.h"

GstRecorder::GstRecorder(quint16 width, quint16 height, quint16 fps,
                         QString device, QObject *parent)
    : QObject(parent),
      videoWitdhPx(width),
      videoHeightPx(height),
      framerate(fps),
      devicepath(device) {
  QGst::init();
  deviceNumber = getDeviceNumber(devicepath);
  qDebug() << "GstRecoder: Initilaizing...";
  if (QGst::ElementFactory::find("omxh264enc")) {
    usesOmx = true;
    qDebug() << "GstRecoder: using OpenMAX";
  }
}

GstRecorder::~GstRecorder() {}

QGst::BinPtr GstRecorder::createVideoSrcBin() {
  QGst::BinPtr videoBin;

  try {
    videoBin = QGst::Bin::create();
    QGst::ElementPtr src = QGst::ElementFactory::make("v4l2src");
    src->setProperty("device", devicepath);

    qDebug() << "GstRecorder: v4l2src with device: " << devicepath;

    QGst::ElementPtr encoder;
    QGst::ElementPtr capsfilter =
        createCapsFilter(videoWitdhPx, videoHeightPx, framerate);

    QGst::ElementPtr parse = QGst::ElementFactory::make("h264parse");
    QGst::ElementPtr queue = QGst::ElementFactory::make("queue");

    if (usesOmx) {
      encoder = QGst::ElementFactory::make("omxh264enc");
    } else {
      encoder = QGst::ElementFactory::make("x264enc");
      qDebug() << "GstRecoder: created x264enc";
    }

    videoBin->add(src, capsfilter, encoder, parse, queue);
    videoBin->linkMany(src, capsfilter, encoder, parse, queue);

    qDebug() << "GstRecorder: createVideoSrcBin: added and linked the elements";

    QGst::PadPtr encoderSrcPad = encoder->getStaticPad("src");
    videoBin->addPad(QGst::GhostPad::create(encoderSrcPad, "src"));

  } catch (const QGlib::Error &error) {
    qDebug() << "Failed to create video source bin:" << error;
    return QGst::BinPtr();
  }
  return videoBin;
}

QGst::BinPtr GstRecorder::createAudioSrcBin() {
  QGst::BinPtr audioBin;
  qDebug() << "creating Audio Source bin.";

  try {
    audioBin = QGst::Bin::fromDescription(
        "autoaudiosrc name=\"audiosrc\" ! audioconvert ! "
        "audioresample ! audiorate ! speexenc ! queue");
  } catch (const QGlib::Error &error) {
    qCritical() << "Failed to create audioSrcBin: " << error;
    return QGst::BinPtr();
  }

  QGst::ElementPtr src = audioBin->getElementByName("audiosrc");
  //autoaudiosrc creates the actual source in the READY state
  src->setState(QGst::StateReady);

  return audioBin;
}

QGst::ElementPtr GstRecorder::createCapsFilter(const quint16 width,
                                               const quint16 height,
                                               const quint16 fps) {
  qDebug() << "creating capsfilter with width: " << width
           << ", height: " << height << ", fps: " << fps;
  QGst::ElementPtr capsfilter;
  try {
    capsfilter = QGst::ElementFactory::make("capsfilter");

    const QGst::CapsPtr caps = QGst::Caps::createSimple("video/x-raw");
    caps->setValue("width", (int)width);
    caps->setValue("height", (int)height);
    caps->setValue("framerate", QGst::Fraction((int)fps, 1));

    capsfilter->setProperty("caps", caps);
  } catch (const QGlib::Error &error) {
    qCritical() << "Failed to create a capsfilter: " << error;
    return QGst::ElementPtr();
  }
  return capsfilter;
}

quint16 GstRecorder::getDeviceNumber(QString path) {
  QRegExp rx("(\\d+)");
  rx.indexIn(path);

  return rx.cap(1).toInt();
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

const QString GstRecorder::recordLocally() {
  QGst::BinPtr videoSrcBin = createVideoSrcBin();
  QGst::BinPtr audioSrcBin = createAudioSrcBin();
  QGst::ElementPtr mux = QGst::ElementFactory::make("mp4mux");
  QGst::BinPtr videoMuxBin = createVideoMuxBin();
  QGst::ElementPtr sink = QGst::ElementFactory::make("filesink");

  QString currentTime =
      QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

  const QString filename = QDir::current().absoluteFilePath(
      currentTime + "-device-" + QString::number(deviceNumber) + ".mp4");
  qDebug() << "GstRecorder: writing to:" << filename;
  sink->setProperty("location", filename);

  if (!videoSrcBin || !sink) {
    qDebug() << "Error. One or more elements could not be created.";
    return "";
  }
  if (m_pipeline) {
    qDebug() << "Another Recording was not started. Already one in progress";
    return "";
  }

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(videoSrcBin, audioSrcBin, mux, sink);

  QGst::PadPtr videoPad = mux->getRequestPad("video_%u");
  QGst::PadPtr audioPad = mux->getRequestPad("audio_%u");

  videoSrcBin->getStaticPad("src")->link(videoPad);
  audioSrcBin->getStaticPad("src")->link(audioPad);

  mux->link(sink);

  m_pipeline->bus()->addSignalWatch();
  QGlib::connect(m_pipeline->bus(), "message", this,
                 &GstRecorder::onBusMessage);

  m_pipeline->setState(QGst::StatePlaying);

  return filename;
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
  emit pipelineCleared();
  qDebug() << "recording stopped";
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
