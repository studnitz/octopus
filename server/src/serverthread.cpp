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
  QByteArray data;
  QJsonObject json;
  while (!socket->atEnd()) {
    data = socket->readLine();
    json = QJsonDocument::fromJson(data).object();
    readData(json);
    // qDebug() << data;
  }
}

void ServerThread::readData(QJsonObject json) {
  qDebug() << " new data";
  QJsonObject o = json["data"].toObject();
  ClientIP = o["IP"].toString();
  clientName = o["Name"].toString();
  clientCpuUsage = o["CPU"].toDouble();
  clientMemUsage = o["Memory"].toDouble();
  clientDiskUsage = o["Disk"].toDouble();
}
void ServerThread::sendCommand(QJsonObject json) {
  QByteArray message;
  QJsonDocument jsonDoc(json);
  message = jsonDoc.toJson(QJsonDocument::Compact).append("\n");
  ;
  socket->write(message);
}

void ServerThread::disconnected() {
  qDebug() << socketDescriptor << " Disconnected";
  socket->deleteLater();
  exit(0);
}
