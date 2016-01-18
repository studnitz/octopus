#include "server.h"
Server::Server(QObject* parent) : QTcpServer(parent) {
  qDebug() << "Server created";
}

Server::~Server() { qDebug() << "Destroy Server"; }

void Server::startServer() {
  int port = 1234;

  if (!this->listen(QHostAddress::LocalHost, port)) {
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
  //connect(thread, SIGNAL(newInfo()), this, SLOT(readInfo()));
  thread->start();

  qDebug() << "New Client connected";
}

void Server::getInfo() { this->broadcastCommand(0); }

void Server::readInfo() {
  QList<ServerThread*> clients = getClients();
  QListIterator<ServerThread*> it(clients);
  emit gotInfo();
  while (it.hasNext()) {
    for (int i = 0; i < 6; ++i) {
      qDebug() << it.peekNext()->ClientInfo[i];
    }
  }
}

QList<ServerThread*> Server::getClients() {
  return findChildren<ServerThread*>();
}

int Server::getNumClients() { return getClients().size(); }
