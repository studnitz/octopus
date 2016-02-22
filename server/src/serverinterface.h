#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>

class ServerInterface : public QTcpServer {
 public:
  ServerInterface(QObject *parent = 0);
  void start(quint16 port = 1235);

 private:
  void incomingConnection(qintptr handle);
  void readData();
  void readJson(const QJsonObject &json);
  QJsonObject writeJson();
  QTcpSocket *socket;
};

#endif  // SERVERINTERFACE_H
