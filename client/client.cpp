#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&client, SIGNAL(connected()), this, SLOT(sendInfo()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
}

Client::~Client() { client.close(); }

void Client::start(QString address, quint16 port) {
  QHostAddress addr(address);
  client.connectToHost(addr, port);
  if (client.waitForConnected()) {
    qDebug() << "Client connected";

  } else {
    qDebug() << "Client not connected";
  }
}

void Client::waitForOrder() { QByteArray Data = client.readAll(); }

void Client::sendInfo() { client.write("I'm Client 1", 12); }

QTcpSocket::SocketState Client::getState() const { return client.state(); }
