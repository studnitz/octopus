#include <QCoreApplication>
#include "myserver.h"
#include "mythread.h"
#include <iostream>


MyServer server;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Make a server and starts it

    server.startServer();



    return a.exec();
}


std::string clients(){
    return server.findChildren<MyThread*>().at(0)->socket->peerAddress().toString().toStdString();
}
