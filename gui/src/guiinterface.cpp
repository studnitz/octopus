#include "guiinterface.h"
#include <iostream>

GUIInterface::GUIInterface(QHostAddress destAddr, quint16 port, QObject *parent)
    : QObject(parent) {
  clients = new QList<ClientGui *>();
  socket = new QTcpSocket(this);

  tryConnect(destAddr, port);
}

void GUIInterface::tryConnect(QHostAddress destAddr, quint16 port) {
  if (socket->state() != QTcpSocket::ConnectedState) {
    socket->connectToHost(destAddr, port);
  }
}

void GUIInterface::sendData(QString str, QString &data) {
  if (socket->state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    msg = newCommand(str, data).toJson(QJsonDocument::Compact).append("\n");
    socket->write(msg);
  }
}

void GUIInterface::receiveData() {
  QByteArray data;
  QJsonObject json;
  clients->clear();
  while (!socket->atEnd()) {
    data = socket->readLine();
    json = QJsonDocument::fromJson(data).object();
    readData(json);
    // qDebug() << data;
  }
}

void GUIInterface::readData(QJsonObject json) {
  if (json["data"].toObject()["clients"].isArray()) {
    qDebug() << " new data";
    QJsonArray arr = json["data"].toObject()["clients"].toArray();
    while (!arr.empty()) {
      QJsonObject o = arr.takeAt(0).toObject();
      QString IP = o["IP"].toString();
      if (IP.compare("")) {
        float cpu = o["CPU"].toDouble();
        float mem = o["Memory"].toDouble();
        float disk = o["Disk"].toDouble();
        QString name = o["Name"].toString();
        ClientGui *Client = new ClientGui(IP, name, cpu, mem, disk);
        clients->append(Client);
      }
    }
  }
}

QJsonDocument GUIInterface::newCommand(QString &cmd, QString &data) {
  QJsonObject json = QJsonObject();
  json["cmd"] = cmd;
  json["data"] = data;
  return QJsonDocument(json);
}
