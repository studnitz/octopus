#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

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
  int ClientInfo[10];
  QTcpSocket* socket;
  qintptr socketDescriptor;
 signals:
  void error(QTcpSocket::SocketError socketerror);
  void ready();
  void newInfo(int info []);
 public slots:
  void sendCommand(int value);
  void disconnected();
 private slots:
  void getData();

};

#endif  // SERVERTHREAD_H
