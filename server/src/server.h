#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QJsonObject>
#include "serverthread.h"
#include "recording.h"

/**
 * @brief The Server class inherits from QTcpServer. It is the core of the
 * complete system and also a single-point-of-failure, so be careful with it.
 * Server is responsible for the communication with the clients. Also it
 * receives information from the GUI (to be precise the GUI has a GuiInterface
 * object, to which it passes information. The Gui Interface Object then
 * communicates via TCP with the ServerInterface object. The ServerInterface
 * object again has a reference to this class (Server), so it can pass the
 * information on again.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class Server : public QTcpServer {
  Q_OBJECT

 public:
  /**
   * @brief Standard constructor for a server object.
   * @param parent obvious, isn't it?
   */
  Server(QObject *parent = 0);
  /**
   * @brief getNumClients counts the amount of Clients currently connected to
   * the server by counting the Number of existing ServerThreads.
   * @return Number of clients connected to the server
   */
  int getNumClients();

  /**
   * @brief getHostname
   * Retrieves the local hostname. Is probably "server", but for stability reasons it is read dynamically.
   * @return hostname
   */
  QString getHostname();

  /**
   * @brief startServer starts the server. From then on the server is listening
   * for incoming connections. By default on port 1234.
   * @param port The default port to be listened to (default=1234)
   */
  void startServer(int port = 1234);

  /**
   * @brief getClients returns a list of ServerThreads containing the currently
   * connected clients.
   * @return list of ServerThreads containing the connected clients
   */
  QList<ServerThread *> getClients();

  Recording *rec;

  ServerThread *getClientByHostname(QString hostname);
  void updateRecording();
  void downloadFiles();

  /**
   * @brief stopCameras broadcasts the String "stopCameras" via broadcastCommand
   * to the clients, making them stop their current recording.
   */
  void stopCameras();

  /**
   * @brief recordLocally broadcasts the String "recordLocally" via
   * broadcastCommand to the clients, making them start a recording and save it
   * locally on their hard drive.
   */
  void recordLocally();

 signals:

  /**
   * @brief broadcastCommand is connected to the sendData slot of the individual
   * ServerThreads
   * @param json contains the command in json-format to be sent to the clients
   * to
   */
  void broadcastCommand(QJsonObject json);

 public slots:
  /**
   * @brief getInfo broadcasts the String "getInfo" via broadcastCommand to the
   * clients, making a request to send their current status-information to the
   * server. It is started when the ready-signal is emitted by ServerThread.
   */
  void getInfo();

 protected:
  /**
   * @brief incomingConnection overrides QTcpServer's incomingConnection method
   * that is called every time a client wants to connect to the server (but
   * hasn't yet). It starts a new ServerThread for every new connection and
   * connects the neccessary signals to it.
   * @param socketDescriptor a qintptr to identify the socket to communicate on
   * with this one client
   */
  void incomingConnection(qintptr socketDescriptor);
};

#endif  // SERVER_H
