#include <QCoreApplication>
#include <src/client.h>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  client.start(1234);

  return a.exec();
}
