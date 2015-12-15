#include "mainwindow.h"
#include "myserver.h"
#include "mythread.h"
#include <QApplication>

MyServer server;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    server.startServer();

    MainWindow w;
    w.show();

    return a.exec();
}
