#include <QCoreApplication>
#include <client.h>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  client.start("127.0.0.1", 1234);

  // client.start("127.0.0.1", 1234);
  return a.exec();
}
