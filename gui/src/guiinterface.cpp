#include "guiinterface.h"
#include <iostream>

GUIInterface::GUIInterface(QHostAddress destAddr, quint16 port, QObject *parent)
    : QObject(parent) {
  clients = new QList<ClientGui*>();
  socket = new QTcpSocket(this);

  socket->connectToHost(destAddr, port);

  if (socket->waitForConnected()) {
    qDebug() << "GUI Interface connected";
  } else {
    qDebug() << "GUI Interface could not connect to Server Interface";
  }
  connect(socket, &QTcpSocket::readyRead, this, &GUIInterface::receiveData);
}

void GUIInterface::readJson(const QJsonObject &json) {}

void GUIInterface::writeJson() {}

void GUIInterface::sendData(QString str, QString &data) {
  if (socket->state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    msg = newCommand(str, data).toJson(QJsonDocument::Compact).append("\n");
    socket->write(msg);
  }
}

void GUIInterface::receiveData(){
    QByteArray data;
    QJsonObject json;
    while(!socket->atEnd()){
        data = socket->readLine();
        json = QJsonDocument::fromJson(data).object();
        readData(json);
    }
}

void GUIInterface::readData(QJsonObject json){
 qDebug() << json["data"].toString();
}

QJsonDocument GUIInterface::newCommand(QString &cmd, QString &data) {
  QJsonObject json = QJsonObject();
  json["cmd"] = cmd;
  json["data"] = data;
  return QJsonDocument(json);
}
