// myserver.cpp

#include "../src/myserver.h"
//#include "../src/mythread.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent) {
  // ONLY FOR 1 CLIENT to show working command broadcast
  connect(this, SIGNAL(newConnection()), this, SLOT(sendCommand()));

  qDebug() << "Server created";
}

void MyServer::startServer() {
  int port = 1234;

  if (!this->listen(QHostAddress::LocalHost, port)) {
    qDebug() << "Could not start server";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }
}

void MyServer::incomingConnection(qintptr socketDescriptor) {
  QTcpSocket *socket;
  socket = new QTcpSocket();

  // set the ID
  if (!socket->setSocketDescriptor(socketDescriptor)) {
    // something's wrong, we just emit a signal
    emit socket->error();
    return;
  }

  qDebug() << "new Connection";
  clients.push_back(socket);
  qDebug() << socketDescriptor
           << "        IP:" << socket->peerAddress().toString()
           << " Client connected";

  // DELETE SOCKET??
}

void MyServer::sendCommand() {
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
      qDebug() << broadcastCommand(command);
      break;

    default:
      std::cout << "command is unknown";
      break;
  }
    }
  return;
}

QString MyServer::broadcastCommand(int command) {
  // should be global const list at end of client connection
  QString answer;
  QByteArray send;
  send.setNum(command);
  for (int i = 0; i < clients.size(); ++i) {  /// TODO QTLISTITERATOR
    answer.append("Client ").append(": ");
    clients.at(i)->write(send);
    clients.at(i)->waitForReadyRead();

    QByteArray data = clients.at(i)->readLine();
    qDebug() << data;
    answer.append(data);
    qDebug() << answer;
    qDebug() << "Order send to Client" << i + 1;
  }
  return answer;
}

int MyServer::getNumClients() { return clients.size(); }

MyServer::~MyServer() { qDebug() << "Destroy Server"; }
