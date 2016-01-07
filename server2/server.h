#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <iostream>

#include "clientconnection.h"

class Server : public QTcpServer {
  Q_OBJECT
 public:
  explicit Server(QObject *parent = 0);
  ~Server();

  void startServer();
 signals:
    void broadcastCommand();
 public slots:
    void sendCommand();

 protected:
  QList<ClientConnection*> connections;
  int getNumClients();
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
