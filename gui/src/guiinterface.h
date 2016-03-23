#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include "clientgui.h"

/**
 * @brief The GUIInterface class is responsible for the communication between
 * the Server and MainWindow. It is beeing used by the MainWindow class.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class GUIInterface : public QObject {
  Q_OBJECT
 public:
  int exportStatus = 0;

  /**
   * @brief Constructor for a GUIInterface
   * @param destAddr The IP-Address to communicate with
   * @param port The port to communicate with the Server on
   * @param parent It's recommended to take MainWindow as parent
   */
  explicit GUIInterface(QHostAddress destAddr, quint16 port,
                        QObject *parent = 0);

  /**
   * @brief tryConnect tries to connect to the given IP-Adress on the given
   * port. Is beeing called multiple times by MainWindow's tryConnection-method
   * @param destAddr The address to be connected to
   * @param port The port to be connected to
   */
  void tryConnect(QHostAddress destAddr, quint16 port);

  /**
   * @brief sendData sends data to the Server. data is a QString, that is
   * converted to JSON before sending it. In comparison with an e-mail str is
   * like the subject and data like the content. Sometimes the subject is
   * sufficient and the content can be left empty, sometimes it's not.
   * @param str name of the command
   * @param data content of the data for the given command
   */
  void sendData(QString str, QJsonObject &data);

  /**
   * @brief clients points to a list of ClientGUi-pointers. It stores all the
   * connected clients.
   */
  QList<ClientGui *> *clients;

  /**
   * @brief socket points to a QTcpSocket that is needed for the communication
   * with the Server.
   */
  QTcpSocket *socket;

  /**
   * @brief newCommand converts the given command and data into a QJsonDocument,
   * so it can be send via the TCP-Connection.
   * @param cmd the command to be converted
   * @param data the data to be converted
   * @return the QJsonDocument containing all the data and cmd
   */
  QJsonDocument newCommand(QString &cmd, QJsonObject &data);

 public slots:

  /**
   * @brief receiveData is called when data comes in. It opens the JSON-Object
   * and executes readData for each sent [command, data]-pair.
   */
  void receiveData();

  /**
   * @brief startExport
   * starts export on the server
   */
  void startExport(QString quality, QString codec);

  /**
   * @brief getExportStatus
   * gets the status of the export from the server.
   */
  void getExportStatus();

 private:
  /**
   * @brief readData decides on what to do and which action to take, depending
   * on the cmd of the json Object
   * @param json The json object that was beeing transmitted and is now to be
   * evaluated
   */
  void readData(QJsonObject json);
};

#endif  // GUIINTERFACE_H
