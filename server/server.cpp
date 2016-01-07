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
  connect(this, &Server::broadcastCommand, client, &ClientConnection::sendCommand);

  connect(this,SIGNAL(setUpSocket()),client,SLOT(setUpSocket()));
  client->moveToThread(thread);
  thread->start();
  emit setUpSocket();
  qDebug() << "New Thread startet";
}

void Server::sendCommand() {
    int  command;

  while (true) {
    std::cout << "Type Command:";
    std::cin >> command;

    switch (command) {
      case 0:
        std::cout << "Number of Clients connected: " << getNumClients() << "\n";
        break;
      case 1:

        std::cout << "Send Command to Clients:";
        std::cin >> command;

        emit this->broadcastCommand(command);

        break;

      default:
        std::cout << "command is unknown";
        break;
    }
  }
  return;
}

int Server::getNumClients() { return findChildren<QThread*>().size(); }
