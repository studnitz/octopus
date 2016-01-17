#include "mainwindow.h"
#include "../server/src/myserver.h"
#include "../server/src/mythread.h"
#include <QApplication>

MyServer server;

int
main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  server.startServer();

  MainWindow w;
  w.server = &server;
  w.show();

  return a.exec();
}
