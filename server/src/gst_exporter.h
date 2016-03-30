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
  QGst::BinPtr createFileSrcBin(const QString path, const int i);

  QGst::BinPtr createVideoMixer();

 signals:

 public slots:

  void exportVideo();

 private:
  quint16 height_;
  quint16 width_;
  bool usesOmx = false;
  Recording* rec_;

  quint16 exportHeightPx;
  quint16 exportWidthPx;

  quint16 elementHeightPx;
  quint16 elementWidthPx;

  QGst::PipelinePtr m_pipeline;

  void onBusMessage(const QGst::MessagePtr& message);

  void stop();
  QGst::BinPtr createEncoder();
  QGst::BinPtr createDecoder(const int i);
  void callbackNewPad(const QGst::ElementPtr &sender, const QGst::PadPtr &pad);
  QGst::ElementPtr createCapsFilter(const quint16 width, const quint16 height);
  char *nameWithIndex(const QString name, const quint16 i);
};

#endif  // GSTEXPORTER_H
