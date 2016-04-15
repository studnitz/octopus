#include "gst_exporter.h"

#include <QDir>
#include <QDebug>
#include <QTimer>

GstExporter::GstExporter(Recording* rec, quint16 widthPx, quint16 heightPx,
                         QObject* parent)
    : QObject(parent),
      rec_(rec),
      exportHeightPx(heightPx),
      exportWidthPx(widthPx) {
  QGst::init();
  if (QGst::ElementFactory::find("omxh264dec")) usesOmx = true;
  rowCount = rec_->grid.height;
  columnCount = rec_->grid.width;
  double elementHeightPxD = 1.0 * exportHeightPx / rowCount;
  double elementWidthPxD = 1.0 * exportWidthPx / columnCount;
  elementHeightPx = (int)elementHeightPxD;
  elementWidthPx = (int)elementWidthPxD;
}

QGst::BinPtr GstExporter::createVideoMixer() {
  qDebug() << "createVideoMixer start";
  QGst::BinPtr videoMixerBin = QGst::Bin::create();

  try {
    QGst::ElementPtr videoMixer =
        QGst::ElementFactory::make("videomixer", "mix");
    videoMixer->setProperty("background", (int) 1); //black background
    videoMixerBin->add(videoMixer);
    int count = 0;
    // go through every element in the grid
    for (int i = 0; i < rec_->grid.height; ++i) {
      for (int j = 0; j < rec_->grid.width; ++j) {
        VideoFile* current = &rec_->grid.grid[i][j];
        if (current->id != 0) {
          qDebug() << "Working on video[" << i << "][" << j
                   << "] with id: " << current->id;

          QGst::PadPtr pad = videoMixer->getRequestPad("sink_%u");
          pad->setProperty("ypos", i * elementHeightPx);
          pad->setProperty("xpos", j * elementWidthPx);
          qDebug() << "Pad created with xpos: " << pad->property("xpos")
                   << ", ypos: " << pad->property("ypos");
          QGst::BinPtr filesrc = createFileSrcBin(current->filepath, count);
          videoMixerBin->add(filesrc);

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
  qDebug() << "createEncoder() start";
  QGst::BinPtr encoder;

  try {
    if (usesOmx) {
      encoder = QGst::Bin::fromDescription("omxh264enc ! h264parse ! mp4mux");
      qDebug() << "Encoder: using omxh264enc";
    } else {
      encoder = QGst::Bin::fromDescription("x264enc ! h264parse ! mp4mux");
      qDebug() << "Encoder: using x264enc";
    }
  } catch (const QGlib::Error& error) {
    qCritical() << "Failed to create encoder bin:" << error;
    return QGst::BinPtr();
  }
  return encoder;
}

QGst::ElementPtr GstExporter::createCapsFilter(const quint16 width,
                                               const quint16 height) {
  qDebug() << "creating capsfilter with width: " << width
           << ", height: " << height;
  QGst::ElementPtr capsfilter;
  try {
    capsfilter = QGst::ElementFactory::make("capsfilter");

    const QGst::CapsPtr caps = QGst::Caps::createSimple("video/x-raw");
    caps->setValue("width", (int)width);
    caps->setValue("height", (int)height);

    capsfilter->setProperty("caps", caps);

  } catch (const QGlib::Error& error) {
    qCritical() << "Failed to create capsfilter: " << error;
    return QGst::ElementPtr();
  }

  return capsfilter;
}

QString GstExporter::exportVideo() {
  qDebug() << "Starting Videoexport...";
  QGst::BinPtr mixer = createVideoMixer();
  QGst::BinPtr encoder = createEncoder();
  QGst::ElementPtr scale = QGst::ElementFactory::make("videoscale");
  QGst::ElementPtr sink = QGst::ElementFactory::make("filesink");
  QGst::ElementPtr capsfilter = createCapsFilter(exportWidthPx, exportHeightPx);
  QString recordingTime = rec_->datetime.toString("yyyy_MM_dd_hh_mm_ss");
  qDebug() << "Loaded recodring, recording time: " << rec_->datetime;

  QDir current = QDir::current();
  current.mkdir("exports");
  current.cd("exports");

  QString filename =
      current.absoluteFilePath("export-" + recordingTime + ".mp4");

  sink->setProperty("location", filename);

  m_pipeline = QGst::Pipeline::create();
  m_pipeline->add(mixer, capsfilter, encoder, scale, sink);
  m_pipeline->linkMany(mixer, capsfilter, scale, encoder, sink);
  qDebug() << "GstExporter: pipeline: Added and linked all elements";

  m_pipeline->bus()->addSignalWatch();
  QGlib::connect(m_pipeline->bus(), "message", this,
                 &GstExporter::onBusMessage);
  qDebug() << "exportVideo(): Starting pipeline";
  m_pipeline->setState(QGst::StatePlaying);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GstExporter::progressPercent);
  timer->start(1000);

  return current.relativeFilePath(filename);
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
        stop(false);
      }
      qCritical()
          << message.staticCast<QGst::ErrorMessage>()->error().message();
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
  quint16 i = rx.cap(1).toInt();

  QString decoderQString = "decoder" + QString::number(i);
  QByteArray decoderBa = decoderQString.toLocal8Bit();
  char* decoderName = decoderBa.data();

  QGst::ElementPtr decoder = m_pipeline->getElementByName(decoderName);

  pad->link(decoder->getStaticPad("sink"));
}

void GstExporter::stop(bool withoutError) {
  m_pipeline->setState(QGst::StateNull);
  m_pipeline.clear();
  timer->stop();
  timer->disconnect();

  emit exportFinished(withoutError);

  qDebug() << "Videoexport finished!";
}

void GstExporter::progressPercent() {
  float result = -1;  // if not active

  if (m_pipeline) {
    QGst::PositionQueryPtr currentPosition =
        QGst::PositionQuery::create(QGst::FormatPercent);
    m_pipeline->query(currentPosition);
    qint64 currentPos = currentPosition->position();
    result = 1.0 * currentPos / 10000;
  }

  emit progressChanged(result);
  qDebug() << "Querying time, result: " << result << "%";

  return;
}

QGst::BinPtr GstExporter::createDecoder(const int i) {
  qDebug() << "createDecoder start, i: " << i;
  char* decArray = nameWithIndex("decoder", i);
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

    // Add Ghostpads for abstraction
    decoderBin->addPad(QGst::GhostPad::create(parserSinkPad, "sink"));
    decoderBin->addPad(QGst::GhostPad::create(decoderSrcPad, "src"));

  } catch (const QGlib::Error& error) {
    qCritical() << "Failed to create a decoder:" << error;
    return QGst::BinPtr();
  }
  return decoderBin;
}

char* GstExporter::nameWithIndex(const QString name, const quint16 i) {
  char* result;
  QString concat = name + QString::number(i);
  QByteArray byteArray = concat.toLocal8Bit();
  result = byteArray.data();

  qDebug() << "nameWithIndex: " << result;

  return result;
}

QGst::BinPtr GstExporter::createFileSrcBin(const QString path, const int i) {
  qDebug() << "creating filesrc bin, path: " << path << " i: " << i;
  QGst::BinPtr videoBin;

  QDir current = QDir::current();
  current.cd("recordings");
  const QString fullPath = current.absoluteFilePath(path);

  try {
    char* srcname = nameWithIndex("file", i);
    QGst::ElementPtr src = QGst::ElementFactory::make("filesrc", srcname);
    src->setProperty("location", fullPath);

    char* demuxName = nameWithIndex("demux", i);
    QGst::ElementPtr demuxer = QGst::ElementFactory::make("qtdemux", demuxName);
    QGst::BinPtr decoder = createDecoder(i);
    QGst::ElementPtr scale = QGst::ElementFactory::make("videoscale");
    QGst::ElementPtr capsfilter =
        createCapsFilter(elementWidthPx, elementHeightPx);

    char* binname = nameWithIndex("filebin", i);
    videoBin = QGst::Bin::create(binname);

    videoBin->add(src, demuxer, decoder, capsfilter, scale);
    src->link(demuxer);
    videoBin->linkMany(decoder, scale, capsfilter);

    qDebug() << "filesrc bin: Added and linked all elements";

    QGst::PadPtr filterPadSrc = capsfilter->getStaticPad("src");
    videoBin->addPad(QGst::GhostPad::create(filterPadSrc, "src"));

    qDebug() << "filesrc bin: Added Ghostpad to the bin";

    QGlib::connect(demuxer, "pad-added", this, &GstExporter::callbackNewPad,
                   QGlib::PassSender);

  } catch (const QGlib::Error& error) {
    qCritical() << "Failed to create a filesource:" << error;
    return QGst::BinPtr();
  }

  return videoBin;
}

GstExporter::~GstExporter() {}
