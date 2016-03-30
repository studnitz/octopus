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
#include <QGst/Message>
#include <QGst/GhostPad>

class GstExporter : public QObject {
  Q_OBJECT
 public:
  explicit GstExporter(Recording* rec, QObject* parent = 0);
  ~GstExporter();
  QGst::BinPtr createFileSrcBin(QString path, int i);

  QGst::BinPtr createVideoMixer();
signals:

 public slots:

  void exportVideo();
private:
  bool usesOmx = false;
  Recording* rec_;
  quint16 elementHeight;
  quint16 elementWidth;
  QGst::PipelinePtr m_pipeline;
  void onBusMessage(const QGst::MessagePtr &message);
  void stop();
  QGst::BinPtr createEncoder();
  QGst::BinPtr createDecoder();
  void callbackNewPad(QGst::PadPtr pad);
};

#endif  // GSTEXPORTER_H
