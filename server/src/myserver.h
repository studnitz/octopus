
#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <iostream>
#include <vector>
#include <string>
#include "../src/mythread.h"

class MyServer : public QTcpServer {
  Q_OBJECT
 public:
  explicit MyServer(QObject *parent = 0);
  ~MyServer();
  void startServer();
  void broadcastOrder(QString order);
  QList<MyThread *> getClients();
   signals:

 public slots:

 protected:
  void incomingConnection(qintptr socketDescriptor);
  QList<QTcpSocket *> connections;


};

#endif  // MYSERVER_H
