#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

#include "serverthread.h"

class Server : public QTcpServer {
  Q_OBJECT

 public:
  explicit Server(QObject *parent = 0);
  ~Server();
  /**
 * @brief getNumClients
 * @return Number of clients connected to server
 */
  int getNumClients();
  /**
   * @brief startServer
   * starts Server
   */
  void startServer();
  /**
   * @brief getClients
   * @return List of ServerThreads which are equal to the connected Clients
   */
  QList<ServerThread *> getClients();

signals:
  /**
   * @brief broadcastCommand
   * @param command command value
   * sends command to all connected Clients
   */
  void broadcastCommand(int command);

 public slots:
  /**
   * @brief getInfo
   * sends Command to all Clients to send new Information
   * is started when ready is emitted by Serverthread
   */
  void getInfo();
  /**
   * @brief readInfo
   * do something with new Information
   * started when new Info is emitted by Serverthread
   */
  void readInfo();

 protected:
  /**
   * @brief incomingConnection
   * @param socketDescriptor
   * starts a new ServerThread where client connection is handeld
   */
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
