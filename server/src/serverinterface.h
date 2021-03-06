#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

#include "server.h"
#include "ftpdownloader.h"
#include "gst_exporter.h"

/**
 * @brief The ServerInterface class is stored on the same physical device as the
 * server and used to communicate with the GUI (or to be precise with the
 * GuiInterface - and yes I know... graphical user interface interface. It
 * wasn't my idea.) Anyway, in this relationship the Server is the slave as all
 * the requests come from the GUI and the server simply answers them. The
 * ServerInterface has a reference to the Server, but not the other way round.
 * @author Bartosz Milejski, Yannick Schädle, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class ServerInterface : public QTcpServer {
 public:
  /**
   * @brief exportStatus contains the current export progress status in percent
   */
  double exportStatus = 0;

  /**
   * @brief ServerInterface is the standard constructor. ServerInterface is
   * usually created in the main()-method of the Server-Application.
   * @param parent a parent
   */
  ServerInterface(QObject *parent = 0);

  /**
   * @brief start starts listening for incoming connections on the given port.
   * @param port the port to be listening to
   */
  void start(quint16 port = 1235);

  /**
   * @brief setServer is the setter-function for the server attribute.
   * @param server obvious, isn't it?
   */
  void setServer(Server *server);

  /**
   * @brief getJsonInfo first collects the IP, name, cpu-, memory- and
   * disk-usage from the clients, then packs it up and returns a JSON-Object
   * containing all of that information.
   * @return a QJsonObject containing all of the above information
   */
  QJsonObject getJsonInfo();

 signals:
  /**
   * @brief rebootClients singal emitted to tell all clients to reboot
   */
  void rebootClients();

 public slots:
  /**
   * @brief exporterProgressChange is used to update the progress on an active
   * export
   * @param value the current progress in percent
   */
  void exporterProgressChange(float value);

  /**
   * @brief exportIsFinished is used when a export is finished (with or without
   * error)
   * @param withoutError true, when the export finished without a error,
   * otherwise false
   */
  void exportIsFinished(bool withoutError);

  /**
   * @brief putFilesToGui tells the ServerInterface to send the recording files
   * to the GUI
   */
  void putFilesToGui();

 private slots:

  /**
   * @brief receiveData is called when new data comes in through the socket.
   * Basically receiveData loops through all of the information it gets and
   * executes executeCommand on it.
   */
  void receiveData();

 private:
  /**
   * @brief incomingConnection connects the socket's readyRead signal with the
   * receiveData slot.
   * @param handle the socket descriptor
   */
  void incomingConnection(qintptr handle);

  /**
   * @brief exportFinished true, when a export has finished
   */
  bool exportFinished = false;

  /**
   * @brief exportError true, when a export has encountered an error
   */
  bool exportError = false;

  QString exportPath;

  /**
   * @brief sendData sends information to GUI as JSON-Object. cmd represents the
   * command and str the actual data beeing sent. If exchange of short
   * information.
   * only is needed, then cmd can be used for message exchange as well.
   * @param cmd the command by which the GUI identifies how to interpret the
   * data (str)
   * @param str the data to be sent to the GUI
   */
  void sendData(QString cmd, QJsonObject &str);

  /**
   * @brief executeCommand launches a command depending on the content of the
   * JSON-Object json. When communicating with the GUI, the messages look like
   * this: message=[cmd, data], where data=somedata or again data=[cmd, data].
   * You can send arbitrary complex Json-Objects like this, each one packed with
   * a command (cmd) that tells the GUI how to interpret the data.
   * executeCommand does exactly that. Depending on the top-level cmd it decides
   * which action to take.
   * @param json that is the data. Basically it looks like this: json=[cmd,data]
   * (in JSON-notation of course)
   */
  void executeCommand(const QJsonObject &json);

  /**
   * @brief socket is a pointer to the QTcpSocket to communicate with the
   * GuiInterface.
   */
  QTcpSocket *socket;

  /**
   * @brief server contains the reference to the Server.
   */
  Server *server;
};

#endif  // SERVERINTERFACE_H
