#include "server.h"
Server::Server(QObject* parent) : QTcpServer(parent) {
  qDebug() << "Server created";
}

Server::~Server() { qDebug() << "Destroy Server"; }

void Server::startServer(int port) {
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
  thread->start();

  qDebug() << "New Client connected";
}

void Server::getInfo() { this->broadcastCommand(0); }

void Server::readInfo() {
  QList<ServerThread*> clients = getClients();
  QListIterator<ServerThread*> it(clients);
  while (it.hasNext()) {
    QVectorIterator<float> cIt(it.next()->ClientInfo);
    while (cIt.hasNext()) {
      qDebug() << cIt.next();
    }
  }
}

QList<ServerThread*> Server::getClients() {
  return findChildren<ServerThread*>();
}

int Server::getNumClients() { return getClients().size(); }
