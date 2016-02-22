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
  socket = new QTcpSocket();
  if (!socket->setSocketDescriptor(handle))
    qDebug() << "Fatal error with socket in ServerInterface";

  connect(socket, &QTcpSocket::readyRead, this, &ServerInterface::readData);
}

void ServerInterface::readData() {
  QByteArray ba = socket->readLine();
  qDebug() << ba;
}

void ServerInterface::readJson(const QJsonObject &json) {}

QJsonObject ServerInterface::writeJson() {}