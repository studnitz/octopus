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
  qDebug()<< "serverthread:: send command 3";
  QByteArray message;
  message.setNum(value);
  socket->write(message);
  qDebug() << "serverthread:: command written 4";
  socket->waitForReadyRead();
  message = socket->readLine();
  qDebug()<< "serverthread:: response read (end of tcp) 5";
  std::stringstream sstr;
   std::string str = message.toStdString();
   float f;
   sstr<<str;
   sstr>>f;
  ClientInfo[value] = (int)f;
}
void ServerThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";

  socket->deleteLater();
  exit(0);
}
