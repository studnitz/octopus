#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&client, SIGNAL(connected()), this, SLOT(sendInfo()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
  connect(&client, SIGNAL(readyRead()), this, SLOT(waitForOrder()));

  // hostfound() signal connect?
}

Client::~Client() {
  qDebug() << "Destroy Client";
  client.close();
}

void Client::start(quint16 port) {
  this->findCamera();

  QHostAddress addr(this->findServer());

  this->syncTime();

  if (timesync) {
    qDebug() << "Time synced";
  } else {
    qDebug() << "Time Server not avaivble";
  }

  client.connectToHost(addr, port);

  if (client.waitForConnected()) {  // Timeout included in waitfor
    qDebug() << "Client connected";

  } else {
    qDebug() << "Client not connected";
  }
  qDebug()<< getState();
  qDebug() << client.isValid();
  qDebug() << client.isOpen();
  qDebug()<< client.isWritable();
  qDebug()<< client.isReadable();

}

void Client::waitForOrder() {
  while (!client.atEnd()) {
    QByteArray Data;
    Data = client.read(3);
    qDebug() << Data;
    if (Data == "000") {
      qDebug() << "GOT ORDER!";
    }
  }
}

void Client::sendInfo() { client.write("123"); }  // INFO ABOUT CLIENT

QTcpSocket::SocketState Client::getState() const { return client.state(); }

QString Client::findServer() {
  /*
   * Find SERVER IP = DHCP
   * NEED TIMEOUT INCLUDED
   *
   */
  QString address = "127.0.0.1";  // HARDCODED

  return address;
}

void Client::syncTime() {
  // HARDCODED
  timesync = true;
  return;
}

void Client::findCamera() {
  qDebug() << "Camera found";
  return;
};
