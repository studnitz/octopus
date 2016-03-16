#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
//#include <QtNetwork> wtf
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QStorageInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gst_recorder.h"

class Client : public QObject {
  Q_OBJECT
 public:
  explicit Client(QObject *parent = 0);
  ~Client();
  /**
 * @brief start starts Client
 * @param port connect to this port
 *
 * IP is found by found Server
 */
  void start(QString serverIp = "127.0.0.1", quint16 port = 1234);

  /**
   * @brief findServer finds local Server
   * @return ServerIP default localhost or DHCP at eth0
   */
  QHostAddress findServer();
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
  int getFreeDisk();
  /**
   * @brief getTotalDisk
   * @return total disk space in KB
   */
  int getTotalDisk();
  QString getHostname();
  QJsonObject getJsonInfo();
  void sendData(QString cmd, QJsonObject &str);
  QStringList listAllDevices();
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
  GstRecorder recorder;
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
   * @brief isConnected is used to test the connection
   * @return "yes"
   */
  std::string isConnected();
  void executeCommand(QJsonObject json);
};

#endif  // TCPCLIENT_H
