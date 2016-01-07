#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <iostream>
#include <string.h>
#include "clientconnection.h"

class Server : public QTcpServer {
  Q_OBJECT
 public:
  explicit Server(QObject *parent = 0);
  ~Server();
int getNumClients();
  void startServer();
 signals:
    void broadcastCommand(int value);
    void setUpSocket();
 public slots:
    void sendCommand();

 protected:
  QList<ClientConnection*> connections;

  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
