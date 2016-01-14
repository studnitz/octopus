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
  connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));
  ClientIP = socket->peerAddress().toString();
  // We'll have multiple clients, we want to know which is which
  qDebug() << socketDescriptor << "IP:" << ClientIP << " Client connected";
  // emit ready();
  exec();
}

void ServerThread::getData() {
  QByteArray message;
  message = socket->readLine();
  qDebug() << message;
  int messageCode = message.toInt();
  switch (messageCode) {
    case 0:
      qDebug() << "Reading info";
      // READING 7 INFOS from CLIENT MESSAGE
      for (int i = 0; i < 7; ++i) {
        message = socket->readLine();
        ClientInfo[i]=message.toInt();
      }
      emit newInfo();;

      break;
    default:
      break;
  }
}

void ServerThread::sendCommand(int value) {
  qDebug() << "send command";
  QByteArray message;
  message.setNum(value);
  qDebug() << message;
  socket->write(message);
  qDebug() << " command written";
}
void ServerThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";

  socket->deleteLater();
  exit(0);
}
