#ifndef GSTREAMER_H
#define GSTREAMER_H

#include <QObject>
#include <Qt5GStreamer/QGlib/Error>
#include <Qt5GStreamer/QGlib/Connect>
#include <Qt5GStreamer/QGst/Init>
#include <Qt5GStreamer/QGst/ElementFactory>
#include <Qt5GStreamer/QGst/ChildProxy>
#include <Qt5GStreamer/QGst/Pipeline>
#include <Qt5GStreamer/QGst/Pad>
#include <Qt5GStreamer/QGst/Event>
#include <Qt5GStreamer/QGst/Message>
#include <Qt5GStreamer/QGst/Bus>

class GstRecorder : public QObject
{
  Q_OBJECT
public:
  GstRecorder(QObject *parent = 0);

private:
  QGst::BinPtr createVideoSrcBin();

  void start();
  void stop();

  void onBusMessage(const QGst::MessagePtr &message);

private:
  QGst::PipelinePtr m_pipeline;
};

#endif // GSTREAMER_H
