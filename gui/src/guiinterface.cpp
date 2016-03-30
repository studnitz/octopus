#include "guiinterface.h"
#include <iostream>

GUIInterface::GUIInterface(QHostAddress destAddr, quint16 port, QObject *parent)
    : QObject(parent) {
  clients = new QList<ClientGui *>();
  socket = new QTcpSocket(this);
  deviceList_ = new QStringList();

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
          ClientGui *Client = new ClientGui(IP, name, cpu, mem, disk, devices);
          clients->append(Client);
        }
      }
    }
  } else if (json["cmd"] == "getExportStatus") {
    exportStatus = json["data"].toObject()["exportStatus"].toDouble();
    qDebug() << "JSON receiveData, exportStatus: " << exportStatus;
  }
}

QJsonDocument GUIInterface::newCommand(QString &cmd, QJsonObject &data) {
  QJsonObject json = QJsonObject();
  json["cmd"] = cmd;
  json["data"] = data["data"];
  return QJsonDocument(json);
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
