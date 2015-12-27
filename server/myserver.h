
#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <iostream>
#include <vector>
#include <string>
#include <mythread.h>

class MyServer : public QTcpServer {
  Q_OBJECT
 public:
  explicit MyServer(QObject *parent = 0);
  ~MyServer();
  void startServer();
  QList<QTcpSocket *> connections;
 signals:

 public slots:

 protected:
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // MYSERVER_H
