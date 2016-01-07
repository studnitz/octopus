
#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <vector>
#include <string>
#include "mythread.h"

class MyServer : public QTcpServer {
  Q_OBJECT
 public:
  explicit MyServer(QObject *parent = 0);
  ~MyServer();

  void startServer();
  QString broadcastCommand(int command);
  int getNumClients();
 QList<MyThread *> getClients();
 signals:
  void sendO();

 public slots:

 protected slots:
  void sendCommand();

 protected:
  void incomingConnection(qintptr socketDescriptor);
  QList<QTcpSocket *> clients;
};

#endif  // MYSERVER_H
