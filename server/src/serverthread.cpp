#include "serverthread.h"

ServerThread::ServerThread(qintptr ID, QObject *parent) : QThread(parent) {
  this->socketDescriptor = ID;
}
void ServerThread::run() {
  qDebug() << " Thread started";

  socket = new QTcpSocket();

  if (!socket->setSocketDescriptor(this->socketDescriptor)) {
    // something's wrong, we just emit a signal
    emit error(socket->error());
    return;
  }

  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

  ClientIP = socket->peerAddress().toString();

  // We'll have multiple clients, we want to know which is which
  qDebug() << socketDescriptor << "IP:" << ClientIP << " Client connected";
  //emit ready();
  exec();
}
void ServerThread::sendCommand(int value) {
  qDebug() << "send command";
  QByteArray message;
  message.setNum(value);
  qDebug() << message;
  socket->write(message);
  qDebug() << " command written";
  socket->waitForReadyRead();
  message = socket->readLine();
  qDebug() << message;
  ClientInfo[value] = message.toInt();
}
void ServerThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";

  socket->deleteLater();
  exit(0);
}
