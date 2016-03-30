#include "gst_exporter.h"

#include <QDir>
#include <QDebug>

GstExporter::GstExporter(Recording* rec, QObject* parent)
    : QObject(parent), rec_(rec), exportHeightPx(960), exportWidthPx(1280) {
  QGst::init();
  height_ = rec_->grid.height;
  width_ = rec_->grid.width;
  double elementHeightPxD = 1.0 * exportHeightPx / height_;
  double elementWidthPxD = 1.0 * exportWidthPx / width_;
  elementHeightPx = (int)elementHeightPxD;
  elementWidthPx = (int)elementWidthPxD;
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
          pad->setProperty("xpos", i * elementWidthPx);
          pad->setProperty("ypos", j * elementHeightPx);
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
  QGst::CapsPtr caps = QGst::Caps::createSimple("video/x-raw");
  caps->setValue("width", (int)exportWidthPx);
  caps->setValue("height", (int)exportHeightPx);
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

  mixer->link(capsfilter);
  capsfilter->link(scale);
  scale->link(encoder);
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

void GstExporter::callbackNewPad(const QGst::ElementPtr& sender,
                                 const QGst::PadPtr& pad) {
  QString padName = pad->name();
  qDebug() << "new pad created: " << padName;

  QString demuxerName = sender->name();
  QRegExp rx("(\\d+)");
  rx.indexIn(demuxerName);
  int i = rx.cap(1).toInt();

  QString decoderQString = "decoder" + QString::number(i);
  QByteArray decoderBa = decoderQString.toLocal8Bit();
  char* decoderName = decoderBa.data();

  QGst::ElementPtr decoder = m_pipeline->getElementByName(decoderName);

  pad->link(decoder->getStaticPad("sink"));
}

void GstExporter::stop() {
  qDebug() << "stop start";
  m_pipeline->setState(QGst::StateNull);

  m_pipeline.clear();
  qDebug() << "exporting finished";
}

QGst::BinPtr GstExporter::createDecoder(int i) {
  qDebug() << "createDecoder start";
  QString name = "decoder" + QString::number(i);
  QByteArray nameAr = name.toLocal8Bit();
  char* decArray = nameAr.data();
  QGst::BinPtr decoderBin;
  try {
    decoderBin = QGst::Bin::create(decArray);
    QGst::ElementPtr parser = QGst::ElementFactory::make("h264parse");
    QGst::ElementPtr decoder;
    if (usesOmx) {
      decoder = QGst::ElementFactory::make("omxh264dec");
    } else {
      decoder = QGst::ElementFactory::make("avdec_h264");
    }
    decoderBin->add(parser, decoder);
    parser->link(decoder);

    QGst::PadPtr parserSinkPad = parser->getStaticPad("sink");
    QGst::PadPtr decoderSrcPad = decoder->getStaticPad("src");

    decoderBin->addPad(QGst::GhostPad::create(parserSinkPad, "sink"));
    decoderBin->addPad(QGst::GhostPad::create(decoderSrcPad, "src"));

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
    // srcname = file%i
    QString name = "file" + QString::number(i);
    QByteArray nameAr = name.toLocal8Bit();
    char* srcname = nameAr.data();
    QGst::ElementPtr src = QGst::ElementFactory::make("filesrc", srcname);
    src->setProperty("location", fullPath);

    QString demuxName = "demux" + QString::number(i);
    QByteArray demuxAr = demuxName.toLocal8Bit();
    char* demuxCa = demuxAr.data();

    QGst::ElementPtr demuxer = QGst::ElementFactory::make("qtdemux", demuxCa);
    QGst::BinPtr decoder = createDecoder(i);
    QGst::ElementPtr scale = QGst::ElementFactory::make("videoscale");

    QGst::CapsPtr caps = QGst::Caps::createSimple("video/x-raw");
    qDebug() << "elementheight: " << elementHeightPx
             << " element width: " << elementWidthPx;
    caps->setValue("height", (int)elementHeightPx);
    caps->setValue("width", (int)elementWidthPx);

    QGst::ElementPtr capsfilter = QGst::ElementFactory::make("capsfilter");
    capsfilter->setProperty("caps", caps);

    QString binName = "filebin" + QString::number(i);
    QByteArray binNameAr = binName.toLocal8Bit();
    char* binname = binNameAr.data();
    videoBin = QGst::Bin::create(binname);

    videoBin->add(src, demuxer, decoder, capsfilter, scale);
    src->link(demuxer);
    decoder->link(scale);
    scale->link(capsfilter);
    capsfilter->link(scale);

    QGst::PadPtr filterPadSrc = capsfilter->getStaticPad("src");
    videoBin->addPad(QGst::GhostPad::create(filterPadSrc, "src"));

    QGlib::connect(demuxer, "pad-added", this, &GstExporter::callbackNewPad,
                   QGlib::PassSender);

  } catch (const QGlib::Error& error) {
    qDebug() << "Failed to create a filesource:" << error;
    return QGst::BinPtr();
  }

  return videoBin;
}

GstExporter::~GstExporter() {}
