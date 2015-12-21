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
    qDebug()<< "CPU Usage: "<< client.getCpuUsage();
    qDebug()<< "Free Memory: " << c;//Free
    qDebug()<< "Total Memory: "<<b;//All
    qDebug()<< "Memory Usage: "<<(c/(b/1.0))*100;//Percentage used
    qDebug()<< "Disk Usage: "<<client.getDiskUsage();
    qDebug()<< "Free Disk Space" << client.getFreeDisk();
    qDebug()<< "Total Disk Space" << client.getTotalDisk();

    return a.exec();
}

