#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <iostream>
#include <string>
#include <sstream>

class ServerThread : public QThread {
  Q_OBJECT
 public:
  ServerThread(qintptr ID, QObject* parent = 0);
  void run();
  QString ClientIP;
  /**
   * @brief ClientInfo
   * 0 CPU Usage
   * 1 Mem Usage
   * 2 Disk Usage
   * 3 Free Mem
   * 4 all mem
   * 5 free disk
   * 6 total disk
   *
   *
   *
   *
   *
   *
   */
  int ClientInfo[10] = {56,34,12,99,23,33,23,23,2,55};
  QTcpSocket* socket;
  qintptr socketDescriptor;
 signals:
  void error(QTcpSocket::SocketError socketerror);
  void ready();
 public slots:
  void sendCommand(int value);
  void disconnected();
};

#endif  // SERVERTHREAD_H
