#include "mainwindow.h"
#include "../server/server.h"
#include <QApplication>
#include <QObject>

Server server;

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  server.startServer();

  MainWindow w;
  QObject::connect(&w, SIGNAL(getinfo()), &server, SLOT(getInfo()));
  w.server = &server;
  w.show();

  return a.exec();
}
