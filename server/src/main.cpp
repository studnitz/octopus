#include <QCoreApplication>
#include "server.h"
#include "recording.h"
#include <iostream>
Server server;

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);
  server.startServer();
  QString path ="/home/snx/build-octopus-Desktop-Debug/client/2016_01_08_11_18_14.mkv";
  Recording* test = new Recording(QDateTime::currentDateTime(), Grid());
  VideoFile file = VideoFile(1337, false, path, QString("localhost"));
  test->grid.addSource(file, 1,1);
  test->saveRecording();
  return a.exec();
}
