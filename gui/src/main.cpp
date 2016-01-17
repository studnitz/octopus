#include "mainwindow.h"
#include "../server/src/server.h"
#include "../server/src/serverthread.h"
#include <QApplication>

Server server;

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
