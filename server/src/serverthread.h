#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


/**
 * @brief The ServerThread class represents a client inside of the Server. Every
 * client gets one ServerThread exactly. As the name suggests, every
 * ServerThread also gets its own thread. It stores information about the client
 * as well as methods to communicate with it using a TCP-Socket.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class ServerThread : public QThread {
  Q_OBJECT

 public:
  /**
   * @brief ServerThread is the standard constructor. It expects a
   * Socketdescriptor to identify a socket with which it should communicate with
   * the client it represents inside of the server.
   * @param ID the socket descriptor
   * @param parent Parent of the Serverthread. Ideally that's the server.
   */
  ServerThread(qintptr ID, QObject* parent = 0);

  /**
   * @brief run starts the tread, connects the socket and saves the client's IP.
   */
  void run();

  /**
   * @brief ClientIP stores the client's IP.
   */
  QString ClientIP;

  /**
   * @brief clientName stores the client's name.
   */
  QString clientName;

  /**
   * @brief clientCpuUsage stores the client's current CPU usage.
   */
  float clientCpuUsage;

  /**
   * @brief clientMemUsage stores the client's current memory usage.
   */
  float clientMemUsage;

  /**
   * @brief clientDiskUsage stores the client's current disk usage.
   */
  float clientDiskUsage;
  QString clientTime;
  QString clientFilePath;
  QStringList clientDevices;
  /**
   * @brief socket stores a pointer, that points to the QTcpSocket that is used
   * for communication with the client.
   */
  QTcpSocket* socket;

  /**
   * @brief socketDescriptor is the socketDescriptor for the above socket.
   */
  qintptr socketDescriptor;

  /**
   * @brief readData reads the clientInformation from a JSON-Object and saves it
   * in the respecting attributes.
   * @param json the QJsonObject to be read from
   */
  void readData(QJsonObject json);

 signals:

  /**
   * @brief error is the error-signal to be emmited when something bad happens
   * @param socketerror the type of error that occured
   */
  void error(QTcpSocket::SocketError socketerror);

  /**
   * @brief ready is emitted when a thread is newly connected (represents a new,
   * not yet handled incoming connection)
   */
  void ready();

  /**
   * @brief newInfo is emitted when new information about the clients is saved
   */
  void newInfo();

 public slots:

  /**
   * @brief sendCommand sends a command to the client
   * @param value a JSON-Object containing the command (and data) to be sent to
   * a client
   */
  void sendCommand(QJsonObject value);

  /**
   * @brief disconnected kills the thread if the connection is closed
   */
  void disconnected();

 private slots:

  /**
   * @brief getData interprets the data sent from the client
   */
  void getData();
};

#endif  // SERVERTHREAD_H
