#include "serverthread.h"
#include "ftpdownloader.h"
#include <QDir>

ServerThread::ServerThread(qintptr ID, QObject *parent) : QThread(parent) {
  this->socketDescriptor = ID;
}
void ServerThread::run() {
  qDebug() << " Thread started";

  socket = new QTcpSocket();

  if (!socket->setSocketDescriptor(this->socketDescriptor)) {
    // something's wrong, we just emit a signal
    emit this->error(socket->error());
    return;
  }

  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
  connect(socket, SIGNAL(error()), this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));
  ClientIP = socket->peerAddress().toString();

  // We'll have multiple clients, we want to know which is which
  qDebug() << socketDescriptor << "IP:" << ClientIP << " Client connected";

  // Emit ready to start getInfo in Server
  emit ready();
  exec();
}

void ServerThread::getData() {
  QByteArray data;
  QJsonObject json;
  while (!socket->atEnd()) {
    data = socket->readLine();
    json = QJsonDocument::fromJson(data).object();
    readData(json);
  }
}

void ServerThread::readData(QJsonObject json) {
  QJsonObject o = json["data"].toObject();
  if (json["cmd"] == "recordLocally") {
    clientFilePath = o["Filename"].toString();
  } else if (json["cmd"] == "videoReady") {
    downloadFile();
    } else if (json["cmd"] == "removeLastRecording") {
      lastRecordingRemoved = o["fileRemoved"].toBool();
  } else {
    ClientIP = o["IP"].toString();
    clientName = o["Name"].toString();
    clientCpuUsage = o["CPU"].toDouble();
    clientMemUsage = o["Memory"].toDouble();
    clientDiskUsage = o["Disk"].toDouble();
    clientTime = o["Time"].toString();
    QJsonArray clientDevicesArray = o["Devices"].toArray();
    clientDevices = QStringList();
    foreach (const QJsonValue &value, clientDevicesArray) {
      clientDevices.push_back(value.toString());
    }
  }
}

quint16 ServerThread::getVideoId(QJsonObject grid) {
  QJsonArray gridArray = grid["grid"].toArray();
  for (int i = 0; i < gridArray.size(); ++i) {
    QJsonArray rowArray = gridArray[i].toArray();
    for (int j = 0; j < rowArray.size(); ++j) {
      QJsonObject current = rowArray[j].toObject();
      if (current["hostname"] == clientName) {
        return current["id"].toInt();
      }
    }
  }
  return 0;
}

void ServerThread::downloadFinished(QString fullpath) {
  qDebug() << "ServerThread: " << clientName
           << " downloadFinished: " << fullpath;

  emit downloadIsFinished(fullpath, currentId);
  QJsonObject json;
  json["cmd"] = "removeLastRecording";
  QJsonObject data;
  json["data"] = data;
  sendCommand(json);
}

void ServerThread::downloadFile() {
  QString filename = clientName + "-" + clientFilePath;
  filename.replace("/", "-");

  QDir savedir = QDir::current();
  savedir.cd("recordings");

  QString recTimeDir = recordingTime.toString("yyyy_MM_dd_hh_mm_ss");

  savedir.mkdir(recTimeDir);
  savedir.cd(recTimeDir);

  QString fullPath = savedir.absoluteFilePath(filename);

  QString ftpUrl = "ftp://" + ClientIP + clientFilePath;

  qDebug() << "ServerThread " << clientName
           << ": Starting Ftp Download from: " << ftpUrl
           << " writing to: " << fullPath;

  FtpDownloader *dl = new FtpDownloader(this, QUrl(ftpUrl), fullPath);
  connect(dl, &FtpDownloader::isFinished, this,
          &ServerThread::downloadFinished);
  dl->startDownload();
}

void ServerThread::sendCommand(QJsonObject json) {
  if (json["cmd"].toString() == "recordLocally") {
    QJsonObject data = json["data"].toObject();
    QJsonObject grid = data["grid"].toObject();
    recordingTime =
        QDateTime::fromString(data["datetime"].toString(), Qt::ISODate);
    currentId = getVideoId(grid);
    json["data"] = data;
    qDebug() << "ServerThread " << clientName
             << "Sending recordLocally with video id: " << currentId
             << "and recording time" << recordingTime;
  }
  QByteArray message;
  QJsonDocument jsonDoc(json);
  message = jsonDoc.toJson(QJsonDocument::Compact).append("\n");
  socket->write(message);
}

void ServerThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";
  socket->deleteLater();
  exit(0);
}
