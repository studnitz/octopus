#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&socket, SIGNAL(connected()), this, SLOT(sendInfo()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
  connect(&socket, SIGNAL(readyRead()), this, SLOT(waitForCommand()));

  // hostfound() signal connect?
}

Client::~Client() {
  qDebug() << "Destroy Client";
  socket.close();
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

  socket.connectToHost(addr, port);

  if (socket.waitForConnected()) {  // Timeout included in waitfor
    qDebug() << "Client connected";

  } else {
    qDebug() << "Client not connected";
  }
//  qDebug() << getState();
//  qDebug() << socket.isValid();
//  qDebug() << socket.isOpen();
//  qDebug() << socket.isWritable();
//  qDebug() << socket.isReadable();
}

void Client::waitForCommand() {
 // while (!socket.atEnd()) {
    QByteArray command;
    command = socket.read(3);
    qDebug() << command;
    qDebug() << "Command recieved";
    if (command == 0) {
      socket.write("Number of Cameras 0\n");
    } else {
      socket.write("command unknown\n");
    }
  //}
}

void Client::sendInfo() {
  // socket.write("123");
}  // INFO ABOUT CLIENT

QTcpSocket::SocketState Client::getState() const { return socket.state(); }

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
