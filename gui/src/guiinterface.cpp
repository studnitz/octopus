#include "guiinterface.h"
#include <iostream>
#include "mainwindow.h"

GUIInterface::GUIInterface(QHostAddress destAddr, quint16 port, QObject *parent)
    : QObject(parent) {
  clients = new QList<ClientGui *>();
  socket = new QTcpSocket(this);
  deviceList_ = new QStringList();

  serverIp = destAddr.toString();

  tryConnect(destAddr, port);
}

void GUIInterface::tryConnect(QHostAddress destAddr, quint16 port) {
  if (socket->state() != QTcpSocket::ConnectedState) {
    socket->connectToHost(destAddr, port);
  }
}

void GUIInterface::sendData(QString str, QJsonObject &data) {
  if (socket->state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    msg = newCommand(str, data).toJson(QJsonDocument::Compact).append("\n");
    socket->write(msg);
  }
}

void GUIInterface::receiveData() {
  QByteArray data;
  QJsonObject json;
  while (!socket->atEnd()) {
    data = socket->readLine();
    json = QJsonDocument::fromJson(data).object();
    readData(json);
  }
}

void GUIInterface::readData(QJsonObject json) {
  MainWindow *p = qobject_cast<MainWindow *>(this->parent());

  if (json["cmd"] == "getInfo") {
    if (json["data"].toObject()["clients"].isArray()) {
      clients->clear();
      QJsonArray arr = json["data"].toObject()["clients"].toArray();
      while (!arr.empty()) {
        QJsonObject o = arr.takeAt(0).toObject();
        QString IP = o["IP"].toString();
        if (IP.compare("")) {
          float cpu = o["CPU"].toDouble();
          float mem = o["Memory"].toDouble();
          float disk = o["Disk"].toDouble();
          QString name = o["Name"].toString();
          QString currentTime = o["Time"].toString();
          QJsonArray deviceArray = o["Devices"].toArray();
          QStringList devices = QStringList();
          bool changed = false;
          for (int i = 0; i < deviceArray.size(); ++i) {
            QString device = deviceArray[i].toString();
            devices.append(device);
            QString clientDevicename = name + ": " + device;
            if (!deviceList_->contains(clientDevicename)) {
              qDebug() << clientDevicename;
              deviceList_->append(clientDevicename);
              changed = true;
            }
          }
          if (changed) {
            emit deviceListUpdated(*deviceList_);
          }
          ClientGui *Client =
              new ClientGui(IP, name, cpu, mem, disk, devices, currentTime);
          clients->append(Client);
        }
      }
    }
  } else if (json["cmd"] == "getExportStatus") {
    QJsonObject data = json["data"].toObject();
    exportStatus = data["exportStatus"].toDouble();
    exportFinished = data["exportFinished"].toBool();
    exportError = data["exportError"].toBool();
    if (exportFinished && !exportError) {
      emit exportIsFinished();
      QDir current = QDir::current();
      current.mkdir("exports");
      current.cd("exports");
      QString exportPath = data["exportPath"].toString();
      QString fullPath = current.absoluteFilePath(exportPath);

      FtpDownloader *ftp = new FtpDownloader(
          this, QUrl("ftp://" + serverIp + "/exports/" + exportPath), fullPath);
      ftp->startDownload();
    }
    if (exportError) {
      emit exportErrored();
      exportError = false;
    }
    qDebug() << "JSON receiveData, exportStatus: " << exportStatus;
  } else if (json["cmd"] == "getFilesfromServer") {
    QJsonObject recording = json["data"].toObject()["recording"].toObject();
    QString dirName = json["data"].toObject()["timename"].toString();
    downloadFiles(recording, dirName);
    QDir dir = QDir::current();
    dir.cd("recordings");
    QFile f(QString("recordings/" + dirName + ".off"));
    qDebug() << f.fileName();
    f.open(QIODevice::WriteOnly);
    QJsonDocument saveDoc(recording);
    f.write(saveDoc.toJson());
    p->updateRecordingList();
  }
}

QJsonDocument GUIInterface::newCommand(QString &cmd, QJsonObject &data) {
  QJsonObject json = QJsonObject();
  json["cmd"] = cmd;
  json["data"] = data["data"];
  return QJsonDocument(json);
}

void GUIInterface::downloadFiles(QJsonObject recording, QString dirName) {
  QJsonArray grid = recording["grid"].toObject()["grid"].toArray();
  for (int i = 0; i < grid.count(); i++) {
    QJsonArray line = grid.at(i).toArray();
    for (int j = 0; j < line.count(); j++) {
      QJsonObject file = line.at(j).toObject();
      QDir dir = QDir::current();

      FtpDownloader *ftp = new FtpDownloader(
          0,
          QUrl("ftp://192.168.1.1/recordings/" + file["filepath"].toString()),
          dir.absolutePath() + QDir::separator() + "recordings" +
              QDir::separator() + file["filepath"].toString());
      ftp->startDownload();
    }
  }
}

void GUIInterface::getExportStatus() {
  QString cmd = QString("getExportStatus");
  QString data2 = QString("");
  QJsonObject data;
  data["data"] = data2;
  sendData(cmd, data);
}

void GUIInterface::startExport(QString quality, QString codec) {
  QString cmd = "startExport";
  QJsonObject o = QJsonObject();
  QJsonObject data;
  o["codec"] = codec;
  o["quality"] = quality;
  data["data"] = o;
  sendData(cmd, data);
}
