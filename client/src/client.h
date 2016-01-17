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
  /**
   * @brief waitForCommand
   * @description slot started after connection established
  */
  void waitForCommand();

 public:
  bool timesync;
  short numCamera;

  QTcpSocket socket;
  QString findServer();
  void findCamera();
  void syncTime();

  /**
   * @brief Client::isConnected is used to test the connection
   * @return "yes"
   */
  std::string isConnected();
  /**
   * @brief Client::getCpuUsage
   * @return current CPU-Usage in percent
   */
  double getCpuUsage();

  /**
   * @brief Client::getFreeMemory
   * @return free memory in KB
   */
  int getFreeMemory();

  /**
    * @brief Client::getAllMemory
    * @return total memory in KB
    */
  int getAllMemory();
  /**
   * @brief getMemoryUsage
   * @return memory usage in percent
   */
  float getMemoryUsage();
  /**
   * @brief Client::getDiskUsage
   * @return disk usage in percent
   */
  double getDiskUsage();
  /**
   * @brief Client::getFreeDisk
   * @return free disk space in KB
   */
  int getFreeDisk();
  /**
   * @brief Client::getTotalDisk
   * @return total disk space in KB
   */
  int getTotalDisk();
};

#endif  // TCPCLIENT_H
