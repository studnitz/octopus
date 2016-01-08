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
  connect(thread,SIGNAL(ready()),this,SLOT(getInfo()));
  thread->start();

  qDebug() << "New Client connected";
}

void Server::getInfo(){
    if(getNumClients()!=0){
    for (int i = 0; i < 6; ++i) {
        this->broadcastCommand(i);
    }
    QList<ServerThread*> clients = getClients();
    QListIterator<ServerThread*> it(clients);
    while (it.hasNext()){
        for (int i = 0; i < 6; ++i) {
            qDebug() << it.peekNext()->ClientInfo[i];
        }

    }
    }
}

QList<ServerThread*> Server::getClients(){
    return findChildren<ServerThread*>();
}

void Server::sendCommand(int command) { emit this->broadcastCommand(command); }

int Server::getNumClients() { return getClients().size(); }
