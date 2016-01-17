#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>

class Client : public QObject {
  Q_OBJECT
 public:
  explicit Client(QObject *parent = 0);
  ~Client();
  /**
 * @brief start
 * @param port connect to this port
 *
 * IP is found by found Server
 */
  void start(quint16 port);
signals:

 public slots:
  /**
   * @brief getState
   * @return status of Client Socket
   */
  QTcpSocket::SocketState getState() const;
 private slots:
  /**
    * @brief sendInfo
    * sends Informations to Server
    */
  void sendInfo();
  /**
   * @brief getCommand
   * started when ReadyRead is emitted to get new Command
   */
  void getCommand();

 private:
  /**
   * @brief timesync
   * DUMMY
   */
  bool timesync;
  /**
   * @brief numCamera
   * DUMMY
   */
  short numCamera;
  /**
   * @brief socket
   * Client Socket
   */
  QTcpSocket socket;
  /**
   * @brief findServer
   * @return ServerIP
   * DUMMY
   */
  QString findServer();
  /**
   * @brief findCamera
   * DUMMY
   */
  void findCamera();
  /**
   * @brief syncTime
   * DUMMY
   */
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
  long getFreeMemory();

  /**
    * @brief Client::getAllMemory
    * @return total memory in KB
    */
  long getAllMemory();
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
  ulong getFreeDisk();
  /**
   * @brief Client::getTotalDisk
   * @return total disk space in KB
   */
  ulong getTotalDisk();
};

#endif  // TCPCLIENT_H
