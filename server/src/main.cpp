#include <QCoreApplication>
#include "server.h"
#include "recording.h"
#include <iostream>
Server server;

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  server.startServer();
  return a.exec();
}
