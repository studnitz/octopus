#include "serverinterface.h"

ServerInterface::ServerInterface(QObject *parent) : QTcpServer(parent) {}

void ServerInterface::start(quint16 port) {
  if (!this->listen(QHostAddress::Any, port)) {
    qDebug() << "Could not start ServerInterface";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }
}

void ServerInterface::setServer(Server *server) { this->server = server; }

void ServerInterface::incomingConnection(qintptr handle) {
  socket = new QTcpSocket(this);
  if (!socket->setSocketDescriptor(handle))
    qDebug() << "Fatal error with socket in ServerInterface";

  connect(socket, &QTcpSocket::readyRead, this, &ServerInterface::receiveData);
}

void ServerInterface::sendData(QString &str) {
  if (socket->state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    QJsonObject json = QJsonObject();

    json["cmd"] = "clientInfo";
    json["data"] = str;
    msg = QJsonDocument(json).toJson(QJsonDocument::Compact).append("\n");

    socket->write(msg);
  }
}

void ServerInterface::receiveData() {
  QByteArray ba;
  QJsonObject json;
  while (!socket->atEnd()) {
    // Read Json object from socket
    ba = socket->readLine();
    json = QJsonDocument::fromJson(ba).object();

    executeCommand(json);
  }
}

void ServerInterface::executeCommand(const QJsonObject &json) {
  if (!json.isEmpty()) {
    if (json["cmd"].toString().compare("getInfo")) {
      // do getInfo
      qDebug() << "cmd:  " << json["cmd"].toString();
      qDebug() << "data: " << json["data"].toString();
    } else {
      qDebug() << "cmd:  " << json["cmd"].toString();
      qDebug() << "data: " << json["data"].toString();
    }
  }
  QString data = QString("TEST TEST TEST TEST");
  sendData(data);
}

void ServerInterface::writeJson() {}
