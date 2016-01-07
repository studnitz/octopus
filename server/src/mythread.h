#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <iostream>
#include <QTcpServer>
class MyThread : public QThread {
  Q_OBJECT
 public:
  explicit MyThread(qintptr ID, QObject *parent = 0);

  void run();

 private:
  QTcpSocket *socket;
  qintptr socketDescriptor;

 signals:
  void error(QTcpSocket::SocketError socketerror);

  void order();

 public slots:
  void readyRead();
  void disconnected();
  void sendOrder();
  void receiveOrder();
};

#endif  // MYTHREAD_H
