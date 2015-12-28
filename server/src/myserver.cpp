// myserver.cpp

#include "../src/myserver.h"
#include "../src/mythread.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent) {
  qDebug() << "Server created";



}

void MyServer::startServer() {
  int port = 1234;

  if (!this->listen(QHostAddress::LocalHost, port)) {
    qDebug() << "Could not start server";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }

  qDebug() << this->isListening();
  qDebug() << this->hasPendingConnections();

}

// This function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection(qintptr socketDescriptor) {
  // We have a new connection
  qDebug() << socketDescriptor << " Connecting...";

  // Every new connection will be run in a newly created thread
  MyThread *thread = new MyThread(socketDescriptor, this);

  // connect signal/slot
  // once a thread is not needed, it will be beleted later
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

  thread->start();
  qDebug() << "Thread startet";
  qDebug() << this->getClients().size();
}
void MyServer::broadcastOrder(QString order) {
  // should be global const list at end of client connection
  QList<MyThread *> clients = this->getClients();

  for (int i = 0; i < clients.size(); ++i) { /// TODO QTLISTITERATOR
    clients.at(i)->sendOrder(order);
    qDebug() << "Order send to " << i;
  }
}
QList<MyThread *> MyServer::getClients() { return findChildren<MyThread *>(); }


    MyServer::~MyServer() {
  qDebug() << "Destroy Server";
}


