#include <QCoreApplication>
#include "client.h"
#include <iostream>

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  Client client;
  client.start("127.0.0.1", 1234);
  qDebug() << "CPU Usage:      " << client.getCpuUsage();
  qDebug() << "Free Memory:    " << client.getFreeMemory();
  qDebug() << "Total Memory:   " << client.getAllMemory();
  qDebug() << "Memory Usage:   " << client.getMemoryUsage();
  qDebug() << "Disk Usage:     " << client.getDiskUsage();
  qDebug() << "Free Disk Space " << client.getFreeDisk();
  qDebug() << "Total Disk Space" << client.getTotalDisk();

  return a.exec();
}
