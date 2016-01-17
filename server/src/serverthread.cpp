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
  // Actually 7 Informations are saved
  ClientInfo.fill(0, 7);

  // We'll have multiple clients, we want to know which is which
  qDebug() << socketDescriptor << "IP:" << ClientIP << " Client connected";

  // Emit ready to start getInfo in Server
  emit ready();
  exec();
}

void ServerThread::getData() {
  QMutableVectorIterator<float> it(ClientInfo);
  QString data =
      QString(socket->readLine().replace("\n", "").replace("\"", ""));
  int messageCode = data.toInt();
  switch (messageCode) {
    case 0:
      while (it.hasNext() && socket->canReadLine()) {
        data = QString(socket->readLine().replace("\n", "").replace("\"", ""));
        it.next();
        it.setValue(data.toFloat());
      }
      emit newInfo();
      break;
    default:
      break;
  }
}

void ServerThread::sendCommand(int value) {
  QByteArray message;
  message.setNum(value);
  socket->write(message);
}

void ServerThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";
  socket->deleteLater();
  exit(0);
}
