#include "serverinterface.h"

ServerInterface::ServerInterface(QObject *parent) : QTcpServer(parent) {}

void ServerInterface::start(quint16 port) {
  if (!this->listen(QHostAddress::Any, port)) {
    qDebug() << "Could not start ServerInterface";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }
}

void ServerInterface::incomingConnection(qintptr handle) {
  socket = new QTcpSocket(this);
  if (!socket->setSocketDescriptor(handle))
    qDebug() << "Fatal error with socket in ServerInterface";

  connect(socket, &QTcpSocket::readyRead, this, &ServerInterface::receiveData);
}

void ServerInterface::receiveData() {
  QByteArray ba;
  ba = socket->readAll();
  QJsonObject json = readJson(ba);
  qDebug() << json["cmd"].toString();
}

QJsonObject ServerInterface::readJson(const QByteArray &ba) {
  QJsonDocument json(QJsonDocument::fromJson(ba));
  if (json.isObject())
    return json.object();
  else
    qDebug() << "is no object";
}

void ServerInterface::writeJson() {}
