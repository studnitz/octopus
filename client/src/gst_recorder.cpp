#include <QDebug>
#include "gst_recorder.h"

GstRecorder::GstRecorder(QObject *parent) : QObject(parent) {}

QGst::BinPtr GstRecorder::createVideoSrcBin() {
  QGst::BinPtr videoBin;

  try {
    videoBin = QGst::Bin::fromDescription("v4l2src ! autovideosink");
    return videoBin;
  } catch (const QGlib::Error &error) {
    qDebug() << "Failed to create video source bin:" << error;
    return QGst::BinPtr();
  }
}

void GstRecorder::start() {
  QGst::BinPtr videoSrcBin = createVideoSrcBin();

  if (!videoSrcBin) {
    qDebug() << "Error creating video source";
    return;
  }

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(videoSrcBin);

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
