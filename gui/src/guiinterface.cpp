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
    if (socket->waitForConnected()) {
      qDebug() << "GUI Interface connected";
      connect(socket, &QTcpSocket::readyRead, this, &GUIInterface::receiveData);
    } else {
      qDebug() << "GUI Interface could not connect to Server Interface";
    }
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
        QList<float> Info = QList<float>();
        QJsonArray array = o["Info"].toArray();
        for (int i = 0; i < 6; i++) {
          Info.append(array.at(i).toDouble());
        }
        ClientGui *Client = new ClientGui(IP, Info);
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
