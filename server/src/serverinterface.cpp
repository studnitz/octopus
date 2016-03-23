#include "serverinterface.h"

ServerInterface::ServerInterface(QObject *parent) : QTcpServer(parent) {}

void ServerInterface::start(quint16 port) {
  if (!this->listen(QHostAddress::Any, port)) {
    qDebug() << "Could not start ServerInterface";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }
}

void ServerInterface::setServer(Server *server) { this->server = server; }

void ServerInterface::incomingConnection(qintptr handle) {
  socket = new QTcpSocket(this);
  if (!socket->setSocketDescriptor(handle))
    qDebug() << "Fatal error with socket in ServerInterface";

  connect(socket, &QTcpSocket::readyRead, this, &ServerInterface::receiveData);
}

void ServerInterface::sendData(QString cmd, QJsonObject &str) {
  if (socket->state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    QJsonObject json = QJsonObject();

    json["cmd"] = cmd;
    json["data"] = str;
    msg = QJsonDocument(json).toJson(QJsonDocument::Compact).append("\n");
    socket->write(msg);
  }
}

void ServerInterface::receiveData() {
  QByteArray ba;
  QJsonObject json;
  while (!socket->atEnd()) {
    // Read Json object from socket
    ba = socket->readLine();
    json = QJsonDocument::fromJson(ba).object();

    executeCommand(json);
  }
}

QJsonObject ServerInterface::getJsonInfo() {
  QJsonObject jO;
  QList<ServerThread *> clients = server->getClients();
  QListIterator<ServerThread *> it(clients);
  QJsonArray clientArray;
  while (it.hasNext()) {
    ServerThread *serverThread = it.next();
    jO["IP"] = serverThread->ClientIP;
    jO["Name"] = serverThread->clientName;
    jO["CPU"] = serverThread->clientCpuUsage;
    jO["Memory"] = serverThread->clientMemUsage;
    jO["Disk"] = serverThread->clientDiskUsage;
    jO["Filename"] = serverThread->clientFilePath;
    jO["Devices"] = QJsonArray::fromStringList(serverThread->clientDevices);
    clientArray.append(jO);
  }
  QJsonObject json;
  json["clients"] = clientArray;
  return json;
}

void ServerInterface::executeCommand(const QJsonObject &json) {
  if (!json.isEmpty()) {
    qDebug() << json["cmd"] << json["data"];
    if (json["cmd"].toString().compare("getInfo") == 0) {
      // do getInfo
      QJsonObject data = getJsonInfo();
      server->broadcastCommand(json);
      sendData(json["cmd"].toString(), data);
      return;
    } else if (json["cmd"].toString().compare("recordLocally") == 0) {
      QJsonObject obj = json["data"].toObject();
      server->rec = new Recording();
      server->rec->read(obj);
      server->recordLocally();
      server->rec->saveRecording();
    } else if (json["cmd"].toString().compare("stopCameras") == 0) {
      server->updateRecording();
      server->stopCameras();
      server->downloadFiles();
    } else if (json["cmd"].toString().compare("getExportStatus") == 0) {
      exportStatus = (exportStatus + 1) % 100;
      QJsonObject data = QJsonObject();
      data["exportStatus"] = exportStatus;
      sendData(json["cmd"].toString(), data);
    } else if (json["cmd"].toString().compare("startExport") == 0) {
      // TODO Start Export
      exportStatus = 0;
    } else {
      qDebug() << "cmd:  " << json["cmd"].toString();
      qDebug() << "data: " << json["data"].toString();
    }
  }
}
