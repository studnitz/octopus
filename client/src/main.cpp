#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  while (client.getState() == QAbstractSocket::UnconnectedState) {
    client.start("127.0.0.1");
  }
  return a.exec();
}
