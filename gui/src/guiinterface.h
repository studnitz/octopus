#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include "clientgui.h"

class GUIInterface : public QObject {
  Q_OBJECT
 public:
  explicit GUIInterface(QHostAddress destAddr, quint16 port,
                        QObject *parent = 0);
  void tryConnect(QHostAddress destAddr, quint16 port);
  void sendData(QString str, QString &data);
  QList<ClientGui *> *clients;
  QTcpSocket *socket;
  QJsonDocument newCommand(QString &cmd, QString &data);
 signals:

 public slots:

  void receiveData();

 private:
  void readData(QJsonObject json);
};

#endif  // GUIINTERFACE_H
