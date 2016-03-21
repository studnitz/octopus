#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  qDebug() << "Server created";
}

void Server::startServer(int port) {
  if (!this->listen(QHostAddress::Any, port)) {
    qDebug() << "Could not start server";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }
}

void Server::incomingConnection(qintptr socketDescriptor) {
  qDebug() << socketDescriptor;
  ServerThread* thread = new ServerThread(socketDescriptor, this);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  connect(this, &Server::broadcastCommand, thread, &ServerThread::sendCommand);
  connect(thread, SIGNAL(ready()), this, SLOT(getInfo()));
  thread->start();

  qDebug() << "New Client connected";
}

void Server::getInfo() {
  QJsonObject json;
  json["cmd"] = "getInfo";
  broadcastCommand(json);
}

void Server::recordLocally() {
  QJsonObject json;
  json["cmd"] = "recordLocally";
  broadcastCommand(json);
}

void Server::stopCameras() {
  QJsonObject json;
  json["cmd"] = "stopCameras";
  broadcastCommand(json);
}


QList<ServerThread*> Server::getClients() {
  return findChildren<ServerThread*>();
}

int Server::getNumClients() { return getClients().size(); }
