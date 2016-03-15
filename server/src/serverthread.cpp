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

  int enableKeepAlive = 1;
  int fd = socket->socketDescriptor();
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

  int maxIdle = 10; /* seconds */
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

  int count = 3;  // send up to 3 keepalive packets out, then disconnect if no response
  setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

  int interval = 2;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
  setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));

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
