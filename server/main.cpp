#include <QCoreApplication>
#include "myserver.h"
#include "mythread.h"
#include <iostream>

MyServer server;

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  // Make a server and starts it

  server.startServer();

  return a.exec();
}

QList<MyThread *> client_threads() { return server.findChildren<MyThread *>(); }
