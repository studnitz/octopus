#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[]) {
  int currentExitCode = 0;
  QString ip = "192.168.1.1";
  if (argc == 2) {
     ip = argv[1];
    }
  do {
    QCoreApplication a(argc, argv);
    Client client;
    client.setParent(&a);
    while (client.getState() == QAbstractSocket::UnconnectedState) {
      // client.start("192.168.1.1");
      client.start(ip);
      currentExitCode = a.exec();
      qDebug() << "ExitCode: " << currentExitCode;
    }
  } while (currentExitCode == Client::EXIT_CODE_REBOOT);
  return currentExitCode;
}
