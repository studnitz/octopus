#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

class ServerThread : public QThread {
  Q_OBJECT

 public:
  /**
   * @brief ServerThread
   * @param ID socketdiscriptor
   * @param parent
   * Constructor
   */
  ServerThread(qintptr ID, QObject* parent = 0);
  /**
   * @brief run
   * Start Thread
   * connect Socket
   * save ClientIP
   */
  void run();
  /**
   * @brief ClientIP
   * Stores ClientIP
   */
  QString ClientIP;

  QString clientName;
  float clientCpuUsage;
  float clientMemUsage;
  float clientDiskUsage;
  QStringList clientDevices;
  /**
   * @brief socket
   * socket connected to client
   */
  QTcpSocket* socket;
  /**
   * @brief socketDescriptor
   */
  qintptr socketDescriptor;

  void readData(QJsonObject json);
 signals:
  /**
   * @brief error
   * @param socketerror
   * just emit error
   */
  void error(QTcpSocket::SocketError socketerror);

  /**
   * @brief ready
   * emitted when connected an thread startet
   */
  void ready();
  /**
   * @brief newInfo
   * emitted when new Informations are saved
   */
  void newInfo();

 public slots:
  /**
   * @brief sendCommand
   * @param value command value
   * sending command to client
   *
   */
  void sendCommand(QJsonObject value);
  /**
   * @brief disconnected
   * kills thread if connection is closed
   */
  void disconnected();

 private slots:
  /**
   * @brief getData
   * interprets Data sent from Client
   */
  void getData();
};

#endif  // SERVERTHREAD_H
