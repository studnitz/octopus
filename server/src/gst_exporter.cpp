#include "gst_exporter.h"

#include <QDir>
#include <QDebug>

GstExporter::GstExporter(Recording* rec, QObject* parent)
    : QObject(parent), rec_(rec), elementHeight(960), elementWidth(1280) {
  QGst::init();
}

QGst::BinPtr GstExporter::createVideoMixer() {
  qDebug() << "createVideoMixer start";
  QGst::BinPtr videoMixerBin = QGst::Bin::create();

  try {
    QGst::ElementPtr videoMixer =
        QGst::ElementFactory::make("videomixer", "mix");
    videoMixerBin->add(videoMixer);
    int count = 0;
    for (int i = 0; i < rec_->grid.height; ++i) {
      for (int j = 0; j < rec_->grid.width; ++j) {
        VideoFile* current = &rec_->grid.grid[i][j];
        if (current->id != 0) {
          qDebug() << "i: " << i << "j: " << j << "id: " << current->id;

          QGst::PadPtr pad = videoMixer->getRequestPad("sink_%u");
          qDebug() << "pad created";
          pad->setProperty("xpos", i * elementHeight);
          pad->setProperty("ypos", j * elementWidth);
          qDebug() << "pad props set, x: " << pad->property("xpos")
                   << " ypos: " << pad->property("ypos");
          QGst::BinPtr filesrc = createFileSrcBin(current->filepath, count);
          videoMixerBin->add(filesrc);
          QString binname = "file" + QString::number(count);
          QByteArray array2 = binname.toLocal8Bit();
          char* ch = array2.data();
          QString path =
              filesrc->getElementByName(ch)->property("location").toString();
          qDebug() << path;
          QGst::PadPtr sourcepad = filesrc->getStaticPad("src");
          sourcepad->link(pad);
          ++count;
        }
      }
    }
    QGst::PadPtr mixSrcPad = videoMixer->getStaticPad("src");
    videoMixerBin->addPad(QGst::GhostPad::create(mixSrcPad, "src"));

  } catch (const QGlib::Error& error) {
    qDebug() << "Failed to create a videomixer:" << error;
    return QGst::BinPtr();
  }

  return videoMixerBin;
}

QGst::BinPtr GstExporter::createEncoder() {
  qDebug() << "createEnc start";
  QGst::BinPtr encoder;

  try {
    QGst::ElementFactoryPtr encoderTest =
        QGst::ElementFactory::find("omxh264enc");
    if (encoderTest) {
      encoder = QGst::Bin::fromDescription(
          "omxh264enc ! h264parse ! mp4mux ! progressreport");
    } else {
      encoder = QGst::Bin::fromDescription(
          "x264enc ! h264parse ! mp4mux ! progressreport");
      qDebug() << "ENcoder: usxing x264";
    }
  } catch (const QGlib::Error& error) {
    qDebug() << "Failed to create encoder bin:" << error;
    return QGst::BinPtr();
  }
  return encoder;
}

void GstExporter::exportVideo() {
  qDebug() << "exportVideo start";
  QGst::BinPtr mixer = createVideoMixer();
  QGst::BinPtr encoder = createEncoder();
  QGst::ElementPtr scale = QGst::ElementFactory::make("videoscale");
  QGst::ElementPtr sink = QGst::ElementFactory::make("filesink");
  QGst::ElementPtr capsfilter =
      QGst::ElementFactory::make("capsfilter", "filter-mix");
  QGst::CapsPtr caps =
      QGst::Caps::fromString("video/x-raw,width=1280,height=960");
  capsfilter->setProperty("caps", caps);

  QGst::PadPtr sinktest =
      mixer->getElementByName("mix")->getStaticPad("sink_2");
  qDebug() << "TEST LOL: " << sinktest->property("xpos");
  QGst::ElementPtr filetest = mixer->getElementByName("file1");
  qDebug() << "TESTTTT : " << filetest->property("location");

  QString recordingTime = rec_->datetime.toString("yyyy_MM_dd_hh_mm_ss");
  qDebug() << "rec time: " << recordingTime;

  QDir current = QDir::current();
  current.mkdir("exports");
  current.cd("exports");

  QString filename =
      QDir::current().absoluteFilePath("export-" + recordingTime + ".mp4");

  sink->setProperty("location", filename);

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(mixer, capsfilter, encoder, scale, sink);

  mixer->link(encoder);
  // capsfilter->link(scale);
  // scale->link(encoder);
  encoder->link(sink);

  m_pipeline->bus()->addSignalWatch();
  QGlib::connect(m_pipeline->bus(), "message", this,
                 &GstExporter::onBusMessage);
  qDebug() << "starting pipeline";
  m_pipeline->setState(QGst::StatePlaying);
}

void GstExporter::onBusMessage(const QGst::MessagePtr& message) {
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

void GstExporter::callbackNewPad(const QGst::PadPtr pad) {
  QString padName = pad->name();
  qDebug() << "new pad created: " << padName;

  QGst::ElementPtr decoder = createDecoder();
  m_pipeline->add(decoder);
  decoder->syncStateWithParent();

  pad->link(decoder->getStaticPad("sink"));
}

void GstExporter::stop() {
  qDebug() << "stop start";
  m_pipeline->setState(QGst::StateNull);

  m_pipeline.clear();
  qDebug() << "exporting finished";
}

QGst::BinPtr GstExporter::createDecoder() {
  qDebug() << "createDecoder start";
  QGst::BinPtr decoderBin;
  try {
    if (usesOmx) {
      decoderBin = QGst::Bin::fromDescription("h264parse ! omxh264dec");
    } else {
      decoderBin = QGst::Bin::fromDescription("h264parse ! avdec_h264");
    }
  } catch (const QGlib::Error& error) {
    qDebug() << "Failed to create a decoder:" << error;
    return QGst::BinPtr();
  }
  return decoderBin;
}

QGst::BinPtr GstExporter::createFileSrcBin(QString path, int i) {
  qDebug() << "filesrcbin start, path: " << path;
  QGst::BinPtr videoBin;

  QDir current = QDir::current();
  current.cd("recordings");
  QString fullPath = current.absoluteFilePath(path);

  try {
    QGst::ElementPtr src = QGst::ElementFactory::make("filesrc");
    src->setProperty("location", fullPath);
    QGst::ElementPtr demux = QGst::ElementFactory::make("qtdemux");
    QGlib::connect(demux, "pad-added", this, &GstExporter::callbackNewPad);
    videoBin = QGst::Bin::create();
    videoBin->add(src, demux);

  } catch (const QGlib::Error& error) {
    qDebug() << "Failed to create a filesource:" << error;
    return QGst::BinPtr();
  }

  return videoBin;
}

GstExporter::~GstExporter() {}
