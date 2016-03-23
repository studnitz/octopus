#include "gst_exporter.h"

GstExporter::GstExporter(QObject *parent) : QObject(parent) { /*QGst::init();*/ }
/*
QGst::BinPtr GstExporter::createFileSrcBin(QString path) {
  QGst::BinPtr videoBin;

  try {
    // look for omx-encoder to test if we are on RPi
    QGst::ElementFactoryPtr decoder = QGst::ElementFactory::find("omxh264dec");
    if (!encoder) {
      videoBin = QGst::Bin::fromDescription(
          "filesrc location=" + path + " ! qtdemux ! h264parse ! avdec_h264");
    } else {
      videoBin = QGst::Bin::fromDescription(
          "filesrc location=" + path + " ! qtdemux ! h264parse ! omx_h264dec");
    }
    return videoBin;
  } catch(const QGlib::Error &error) {
    qFatal() << "Failed to create a filesource:" << error;
    return QGst::BinPtr();
  }
}*/

GstExporter::~GstExporter() {}
