#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QString>

class Client : public QObject {
  Q_OBJECT
 public:
  explicit Client(QObject *parent = 0);
  ~Client();
  /*
   * start by given port
   */
  void start(quint16 port);
 signals:

 public slots:

  QTcpSocket::SocketState getState() const;

 private slots:

  void sendInfo();
  void waitForCommand();

 private:
  bool timesync;
  short numCamera;
  char* info;
  // Liste Cameras
  QTcpSocket socket;
  QString findServer();
  void findCamera();
  void syncTime();
};

#endif  // TCPCLIENT_H
