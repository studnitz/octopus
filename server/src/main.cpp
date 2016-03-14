#include <QCoreApplication>
#include "server.h"
#include "recording.h"
#include <iostream>
#include "src/serverinterface.h"
Server server;
ServerInterface* serverInterface;

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  server.startServer();

  serverInterface = new ServerInterface(0);
  serverInterface->setServer(&server);
  serverInterface->start();

  return a.exec();
}
