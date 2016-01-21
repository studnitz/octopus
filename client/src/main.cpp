#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  //QGst::init(&argc, &argv);

  Client client;
  client.start();
  while (client.getState() == QAbstractSocket::UnconnectedState) {
    client.start();
  }
  return a.exec();
}
