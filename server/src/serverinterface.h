#ifndef SERVERINTERFACE_H
#define SERVERINTERFACE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

class ServerInterface : public QTcpServer {
 public:
  ServerInterface(QObject *parent = 0);
  void start(quint16 port = 1235);

 private slots:
  void receiveData();

 private:
  void incomingConnection(qintptr handle);
  QJsonObject readJson(const QByteArray &ba);
  void writeJson();
  QTcpSocket *socket;
};

#endif  // SERVERINTERFACE_H
