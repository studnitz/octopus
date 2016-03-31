#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  int currentExitCode = 0;
  do {
    QCoreApplication a(argc, argv);
    Client client;
    client.setParent(&a);
    while (client.getState() == QAbstractSocket::UnconnectedState) {
      // client.start("192.168.1.1");
      client.start();
      currentExitCode = a.exec();
      qDebug() << "ExitCode: " << currentExitCode;
    }
  } while (currentExitCode == Client::EXIT_CODE_REBOOT);
  return currentExitCode;
}
