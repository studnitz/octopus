#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
//  while (client.getState() == QAbstractSocket::UnconnectedState) {
   qDebug() << client.listAllDevices();
   client.start();
 // }
  return a.exec();
}
