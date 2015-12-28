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

class GstRecorder : public QObject {
  Q_OBJECT
public:
  GstRecorder(QObject *parent = 0);

  void start();
  void stop();

private:
  QGst::BinPtr createVideoSrcBin();

  void onBusMessage(const QGst::MessagePtr &message);

  QGst::PipelinePtr m_pipeline;
};

#endif // GST_RECORDER_h
