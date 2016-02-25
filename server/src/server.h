#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "serverthread.h"

class Server : public QTcpServer {
  Q_OBJECT

 public:
  Server(QObject *parent = 0);
  ~Server();
  /**
 * @brief getNumClients
 * @return Number of clients connected to server
 */
  int getNumClients();
  /**
   * @brief startServer starts Server
   * @param port default 1234
   */
  void startServer(int port = 1234);
  /**
   * @brief getClients
   * @return List of ServerThreads which are equal to the connected Clients
   */
  QList<ServerThread *> getClients();

 signals:
  /**
   * @brief broadcastCommand sends command to all connected Clients
   * @param command command value
   */
  void broadcastCommand(int command);
  /**
   * @brief gotInfo
   */
  void gotInfo();

 public slots:
  void recordLocally();
  /**
   * @brief getInfo sends Command to all Clients to send new Information
   * is started when ready is emitted by Serverthread
   */
  void getInfo();
  /**
   * @brief readInfo do something with new Information
   * started when new Info is emitted by Serverthread
   */
  void readInfo();

  void stopRecordings();

 protected:
  /**
   * @brief incomingConnection starts a new ServerThread where client connection
   * is handeld
   * @param socketDescriptor
   */
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
