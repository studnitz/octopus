#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include "clientgui.h"

/**
 * @brief GUIInterface class
 * Class for the Communication between server and GUI.
 * Is used by the MainWindow class.
 */
class GUIInterface : public QObject {
  Q_OBJECT
 public:
  explicit GUIInterface(QHostAddress destAddr, quint16 port,
                        QObject *parent = 0);
  void tryConnect(QHostAddress destAddr, quint16 port);
  void sendData(QString str, QJsonObject &data);
  QList<ClientGui *> *clients;
  QTcpSocket *socket;
  QJsonDocument newCommand(QString &cmd, QJsonObject &data);
 signals:

 public slots:

  void receiveData();

 private:
  void readData(QJsonObject json);
};

#endif  // GUIINTERFACE_H
