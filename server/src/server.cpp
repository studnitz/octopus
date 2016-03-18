#include "server.h"
#include "ftpdownloader.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  qDebug() << "Server created";
}

Server::~Server() { qDebug() << "Destroy Server"; }

void Server::startServer(int port) {
  if (!this->listen(QHostAddress::Any, port)) {
    qDebug() << "Could not start server";
  } else {
    qDebug() << "Listening to port " << serverAddress() << port << "...";
  }
}

void Server::incomingConnection(qintptr socketDescriptor) {
  qDebug() << socketDescriptor;
  ServerThread* thread = new ServerThread(socketDescriptor, this);
  connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
  connect(this, &Server::broadcastCommand, thread, &ServerThread::sendCommand);
  connect(thread, SIGNAL(ready()), this, SLOT(getInfo()));
  // connect(thread, SIGNAL(newInfo()), this, SLOT(readInfo()));
  thread->start();

  qDebug() << "New Client connected";
}

void Server::getInfo() {
  QJsonObject json;
  json["cmd"] = "getInfo";
  broadcastCommand(json);
}

void Server::recordLocally() {
  // TODO(eventuell): Als Parameter (in data) Kamera/Client der aufnehmen soll
  // mitschicken
  QJsonObject json;
  json["cmd"] = "recordLocally";
  broadcastCommand(json);
}

void Server::stopCameras() {
  QJsonObject json;
  json["cmd"] = "stopCameras";
  broadcastCommand(json);
}

void Server::downloadFiles() {
  for (int i = 0; i < rec->grid.height; ++i) {
    for (int j = 0; i < rec->grid.width; ++j) {
      VideoFile* currentVid = &rec->grid.grid[i][j];
      if (currentVid->id != 0) {
        QString ip = getClientByHostname(currentVid->hostname)->ClientIP;
        QString ftpurl = "ftp://" + ip + currentVid->filepath;
        FtpDownloader download(0, QUrl(ftpurl),
                               currentVid->hostname + "-" + currentVid->filepath);
        download.startDownload();
      }
    }
  }
}

QList<ServerThread*> Server::getClients() {
  return findChildren<ServerThread*>();
}

ServerThread* Server::getClientByHostname(QString hostname) {
  foreach (ServerThread* client, getClients()) {
    if (client->clientName == hostname) {
      return client;
    }
  }
  return new ServerThread(-1);
}

void Server::updateRecording() {
  for (int i = 0; i < rec->grid.height; ++i) {
    for (int j = 0; j < rec->grid.width; ++j) {
      qDebug() << "hello" << i << "   " << j;
      VideoFile* currentVid = &rec->grid.grid[i][j];
      if (currentVid->id != 0) {
        ServerThread* client = getClientByHostname(currentVid->hostname);
        currentVid->filepath = client->clientFilePath;
      }
    }
  }
  rec->saveRecording();
}

int Server::getNumClients() { return getClients().size(); }
