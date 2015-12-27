// myserver.cpp

#include "myserver.h"
#include "mythread.h"

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
}

MyServer::~MyServer() { qDebug() << "Destroy Server"; }
