#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <iostream>
#include <string.h>

#include "serverthread.h"

class Server : public QTcpServer {
  Q_OBJECT
 public:
  explicit Server(QObject *parent = 0);
  ~Server();
  int getNumClients();
  void startServer();
 signals:
  void broadcastCommand(int command);
  void setUpSocket();
 public slots:
  void getInfo();
  void sendCommand(int command);

 protected:
  QList<ServerThread*> getClients();
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
