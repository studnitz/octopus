#ifndef GSTEXPORTER_H
#define GSTEXPORTER_H

#include <QObject>
#include <QGlib/Error>
#include <QGlib/Connect>
#include <QGst/Init>
#include <QGst/ElementFactory>
#include <QGst/Pipeline>
#include <QGst/Bus>
#include <QGst/Pad>


class GstExporter : public QObject
{
  Q_OBJECT
public:
  explicit GstExporter(QObject *parent = 0);
  ~GstExporter();
  QGst::BinPtr createFileSrcBin(QString path);

signals:

public slots:
};

#endif // GSTEXPORTER_H
