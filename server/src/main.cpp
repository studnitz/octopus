#include <QCoreApplication>
#include "server.h"

Server server;

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  server.startServer();
  return a.exec();
}
