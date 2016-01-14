#include <QCoreApplication>
#include "client.h"
#include "gst_recorder.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  client.start("127.0.0.1", 1234);

  QGst::init(&argc, &argv);

  return a.exec();
}
