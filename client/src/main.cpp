#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  while (client.getState() == QAbstractSocket::UnconnectedState) {

      // For testing on localhost exchange the 1st following line through the second
      client.start("192.168.1.1");
      //client.start("127.0.0.1");
  }
  return a.exec();
}
