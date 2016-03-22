#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QStorageInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "gst_recorder.h"

/**
 * @brief The Client class represents a client. It is responsible for starting,
 * stopping and manipulating recordings using the GstRecorder class. If a client
 * is not connected yet, it tries to connect to the Server that has a fixed IP
 * inside the local network.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class Client : public QObject {
  Q_OBJECT
 public:
  /**
   * @brief Standard constructor for the Client
   */
  explicit Client(QObject *parent = 0);

  ~Client();

  /**
   * @brief start tries to connect to a given IP on a given port through socket
   * @param serverIp The IP to connect to. When no parameters are specified, the
   * Client assumes it is in Debug mode and looks for the Server on the same
   * machine the client runs (localhost).
   * @param port The port to connect to. Should be left 1234 if no changes on
   * the Server are made as well
   */
  void start(QString serverIp = "127.0.0.1", quint16 port = 1234);

  /**
   * @brief getCpuUsage reads the current CPU-usage from a file that is stored
   * under /proc/stat. Because the directory is fixed, this works only on
   * linux-systems. Tested on Ubuntu, Rasbpian, Archlinux.
   * @return A double value containing the CPU-Usage. If the CPU Usage is
   * 23,45%, getCpuUsage returns 23.45
   */
  double getCpuUsage();

  /**
   * @brief getFreeMemory reads the current memory-usage (RAM) from a file that
   * is stored under /proc/meminfo. Because the directory is fixed, this works
   * only on linux-systems. Tested on Ubuntu, Rasbpian, Archlinux.
   * @return An integer-value containing the amount of unused kilobytes of the
   * random access memory (RAM)
   */
  int getFreeMemory();

  /**
   * @brief getAllMemory reads the total size of the memory (RAM) from a file
   * that is stored under /proc/meminfo. Because the directory is fixed, this
   * works only on linux-systems. Tested on Ubuntu, Rasbpian, Archlinux.
   * @return An integer-value containing the amount of total kilobytes in the
   * random access memory (RAM)
   */
  int getAllMemory();

  /**
   * @brief getMemoryUsage calculates the percentage of memory used by the
   * Client using getFreeMemory and getAllMemory.
   * @return A float value containing the memory-usage. If the memory usage is
   * 23,45%, getMemoryUsage returns 23.45
   */
  float getMemoryUsage();

  /**
   * @brief getDiskUsage calculates the percentage of diskspace used by the
   * Client using getFreeDisk and getTotalDisk.
   * @return A double value containing the disk-usage. If the disk usage is
   * 23,45%, getDiskUsage returns 23.45
   */
  double getDiskUsage();

  /**
   * @brief getFreeDisk reads the current disk-usage.
   * @return An integer-value containing the amount of unused kilobytes in the
   * root directory /.
   */
  int getFreeDisk();

  /**
   * @brief getTotalDisk returns the total disk space of the root directory
   * @return Amount of total kilobytes in the root directory
   */
  int getTotalDisk();

  /**
   * @brief getHostname reads the hostname from /etc/hostname.
   * @return A QString containing the hostname
   */
  QString getHostname();

  /**
   * @brief getJsonInfo reads/calculates all the information stored about a
   * Client and returns a JSON-Object containing it.
   * @return A QJsonObject containing IP, Name, CPU, Memory, Disk - Attributes
   */
  QJsonObject getJsonInfo();

  /**
   * @brief sendData sends a message to the server in the form a QJsonObject.
   * cmd and str are packed again into msg([cmd|str]), then sent.
   * @param cmd The command that tells the Server what to do with the data
   * @param str The data
   */
  void sendData(QString cmd, QJsonObject &str);

 public slots:

  /**
   * @brief getState returns socket.state()
   * @return status of client socket
   */
  QTcpSocket::SocketState getState() const;

 private slots:

  /**
   * @brief getCommand is started every time when readyRead is emmited.
   * getCommand also calls executeCommand on the received object to decide what
   * to do next.
   */
  void getCommand();

 private:
  /**
   * @brief recorder holds the GstRecorder, that is our Gstreamer backend. All
   * the Video-Operations like recording, cutting, manipulating etc are made
   * with Gstreamer in octopus.
   */
  GstRecorder recorder;

  /**
   * @brief socket holds the client's TCP-Socket for the
   * Client-Server-communication
   */
  QTcpSocket socket;

  /**
   * @brief executeCommand decides depending on command what to do with the data
   * @param json The JSON object json=[command|data]
   */
  void executeCommand(QJsonObject json);
};

#endif  // TCPCLIENT_H
