#include <QCoreApplication>
#include "client.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;
    client.start("127.0.0.1", 1234);
    long c = client.getFreeMemory();
    long b = client.getAllMemory();
    qDebug()<< client.getCurrentValue();
    qDebug()<< client.getCurrentValue();
    qDebug()<< client.getCurrentValue();
    qDebug()<< c;//Free
    qDebug()<< b;//All
    qDebug()<< (c/(b/1.0))*100;//Percentage used

    return a.exec();
}

