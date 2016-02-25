#include "guiinterface.h"
#include <iostream>

GUIInterface::GUIInterface(QHostAddress destAddr, quint16 port, QObject *parent)
    : QObject(parent) {
  socket = new QTcpSocket(this);

  socket->connectToHost(destAddr, port);

  if (socket->waitForConnected()) {
    qDebug() << "GUI Interface connected";
  } else {
    qDebug() << "GUI Interface could not connect to Server Interface";
  }
}

void GUIInterface::readJson(const QJsonObject &json) {}

void GUIInterface::writeJson() {}

void GUIInterface::sendData(QString &str) {
  if (socket->state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    msg = newCommand(str).toJson();
    socket->write(msg);
  }
}

QJsonDocument GUIInterface::newCommand(QString cmd) {
  QJsonObject json = QJsonObject();
  json["cmd"] = cmd;
  json["blablu"] = 0.7;
  std::cout << QString(QJsonDocument(json).toJson()).toStdString();
  return QJsonDocument(json);
}
