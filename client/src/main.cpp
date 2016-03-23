#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  while (client.getState() == QAbstractSocket::UnconnectedState) {
    client.start("192.168.1.1");
    client.start();
  }
  return a.exec();
}
