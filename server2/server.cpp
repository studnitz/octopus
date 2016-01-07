#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  connect(this, SIGNAL(newConnection()), this, SLOT(sendCommand()));
  qDebug() << "Server2 created";
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
  QThread* thread = new QThread(this);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

  ClientConnection* client = new ClientConnection(socketDescriptor);
  connect(this, SIGNAL(broadcastCommand()), client, SLOT(writeData()));
  client->moveToThread(thread);
  //connections.push_back(client);
  qDebug() << client->thread() << "ClientCon Thread";
  qDebug() << this->thread()<< "Server thread";
  thread->start();
  qDebug() << "New Connection";
}

void Server::sendCommand() {
  qint32 command;

  while (true) {
    std::cout << "Type Command:";
    std::cin >> command;

    switch (command) {
      case 0000:
        std::cout << "Number of Clients connected: " << getNumClients() << "\n";
        break;
      case 0001:

        std::cout << "Send Command to Clients \n";
        std::cout << "Type your Command \n";
        std::cin >> command;
        emit this->broadcastCommand();
        break;

      default:
        std::cout << "command is unknown";
        break;
    }
  }
  return;
}

int Server::getNumClients() { return findChildren<ClientConnection*>().size(); }
