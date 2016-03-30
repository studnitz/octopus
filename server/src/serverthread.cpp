#include "serverthread.h"

ServerThread::ServerThread(qintptr ID, QObject *parent) : QThread(parent) {
  this->socketDescriptor = ID;
}
void ServerThread::run() {
  qDebug() << " Thread started";

  socket = new QTcpSocket();

  if (!socket->setSocketDescriptor(this->socketDescriptor)) {
    // something's wrong, we just emit a signal
    emit this->error(socket->error());
    return;
  }

  connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
  connect(socket, SIGNAL(error()), this, SLOT(disconnected()));
  connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));
  ClientIP = socket->peerAddress().toString();

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
  }
}

void ServerThread::readData(QJsonObject json) {
  QJsonObject o = json["data"].toObject();
  if (json["cmd"] == "recordLocally") {
    clientFilePath = o["Filename"].toString();
  } else {
    ClientIP = o["IP"].toString();
    clientName = o["Name"].toString();
    clientCpuUsage = o["CPU"].toDouble();
    clientMemUsage = o["Memory"].toDouble();
    clientDiskUsage = o["Disk"].toDouble();
    QJsonArray clientDevicesArray = o["Devices"].toArray();
    clientDevices = QStringList();
    foreach (const QJsonValue &value, clientDevicesArray) {
      clientDevices.push_back(value.toString());
    }
  }
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
