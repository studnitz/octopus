#include <QCoreApplication>
#include "server.h"

Server server;

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  // Make a server and starts it
  server.startServer();
  return a.exec();
}
