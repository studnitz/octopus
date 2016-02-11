#include "mainwindow.h"
#include "../server/src/server.h"
#include "../server/src/serverthread.h"
#include "../server/src/recording.h"
#include <QApplication>

Server server;

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  server.startServer();

  MainWindow w;
  QObject::connect(&w, SIGNAL(getinfo()), &server, SLOT(getInfo()));
  // QObject::connect(&server, SIGNAL(gotInfo()), &w,
  // SLOT(continueUpdateClientList()));
  w.server = &server;
  w.show();

  return a.exec();
}
