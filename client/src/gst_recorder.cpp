#include <QDebug>
#include <QDir>
#include <QDateTime>
#include "gst_recorder.h"

GstRecorder::GstRecorder(QObject *parent) : QObject(parent) {}

QGst::BinPtr GstRecorder::createVideoSrcBin() {
  QGst::BinPtr videoBin;

  try {
    QGst::ElementFactoryPtr encoder = QGst::ElementFactory::find("omxh264enc");
    if (!encoder) {
      // if we don't have omx (usually we are not on a RPI), use x264enc instead
      videoBin = QGst::Bin::fromDescription(
          "v4l2src device=/dev/video1 ! x264enc ! h264parse ! matroskamux");
      qDebug() << "Using x264enc on device /dev/video1";
    } else {
      videoBin = QGst::Bin::fromDescription(
          "v4l2src ! omxh264enc ! h264parse ! matroskamux");
      qDebug() << "Using omxh264enc";
    }

    return videoBin;

  } catch (const QGlib::Error &error) {
    qDebug() << "Failed to create video source bin:" << error;
    return QGst::BinPtr();
  }
}

void GstRecorder::start() {
  QGst::BinPtr videoSrcBin = createVideoSrcBin();
  QGst::ElementPtr sink = QGst::ElementFactory::make("filesink");

  QString currentTime =
      QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

  QString filename =
      QDir::currentPath() + QDir::separator() + currentTime + ".mkv";
  qDebug() << "writing to:" << filename;
  sink->setProperty("location", filename);

  if (!videoSrcBin || !sink) {
    qDebug() << "Error. One or more elements could not be created.";
    return;
  }

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(videoSrcBin, sink);

  videoSrcBin->link(sink);

  m_pipeline->bus()->addSignalWatch();
  QGlib::connect(m_pipeline->bus(), "message", this,
                 &GstRecorder::onBusMessage);

  m_pipeline->setState(QGst::StatePlaying);
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
