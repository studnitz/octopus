#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

#include <QtNetwork>
#include <QTcpSocket>
#include <QDebug>
#include <QString>
#include <iostream>

class Client : public QObject
{
  Q_OBJECT
public:
  explicit Client(QObject* parent = 0);
  ~Client();
  void start(QString address, quint16 port);
signals:

public slots:
  void startTransfer();
  double getCpuUsage();
  long getFreeMemory();
  long getAllMemory();
  double getDiskUsage();
  ulong getFreeDisk();
  ulong getTotalDisk();
  std::string isConnected();

private:
  QTcpSocket client;
};

#endif // TCPCLIENT_H
