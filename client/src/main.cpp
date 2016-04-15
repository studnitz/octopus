#include <QCoreApplication>
#include "client.h"
#include <QProcess>

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
      client.start();
    }
    currentExitCode = a.exec();
  } while (currentExitCode == Client::EXIT_CODE_REBOOT);
  return currentExitCode;
}
