#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <src/server.h>

class ServerInterface : public QTcpServer {
 public:
  ServerInterface(QObject *parent = 0);
  void start(quint16 port = 1235);
  void setServer(Server *server);

 private slots:
  void receiveData();

 private:
  void incomingConnection(qintptr handle);
  void sendData(QString &str);
  void executeCommand(const QJsonObject &json);
  void writeJson();
  QTcpSocket *socket;
  Server *server;
};

#endif  // SERVERINTERFACE_H
