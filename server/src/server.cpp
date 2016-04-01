#include "server.h"

Server::Server(QObject* parent) : QTcpServer(parent) {
  qDebug() << "Server created";
  connect(this, &Server::allDownloadsFinished, &Server::onAllDownloadsFinished);
}

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
  connect(thread, &ServerThread::downloadIsFinished, this,
          &Server::updateEntryById);
  connect(thread, SIGNAL(ready()), this, SLOT(getInfo()));
  thread->start();

  qDebug() << "New Client connected";
}

void Server::getInfo() {
  QJsonObject json;
  json["cmd"] = "getInfo";
  broadcastCommand(json);
}

void Server::recordLocally(QJsonObject& settings) {
  QJsonObject json;
  json["cmd"] = "recordLocally";
  json["data"] = settings;
  broadcastCommand(json);
}

void Server::stopCameras() {
  updatedCount = 0;
  QJsonObject json;
  json["cmd"] = "stopCameras";
  broadcastCommand(json);
}

void Server::rebootClients() {
  QJsonObject json;
  json["cmd"] = "reboot";
  broadcastCommand(json);
}

void Server::updateEntryById(const QString fullpath, const quint16 id) {
  VideoFile* current = &rec->grid.getVideoFileById(id);
  current->filepath = fullpath;
  current->isRemote = false;
  QPair<int, int> pos = rec->grid.getVideoFilePositionById(id);
  rec->grid.grid[pos.first][pos.second];
  qDebug() << "Server updateEntryById: id " << id << "path: " << fullpath;

  updatedCount++;
  qDebug() << "Server updated VideoFile count increased: " << updatedCount;

  if (updatedCount == rec->grid.size()) {
    emit allDownloadsFinished();
      qDebug() << "Server allDownloadsFinished emitted";
  }

}

void Server::onAllDownloadsFinished() {
  rec->saveRecording();
}

void Server::downloadFiles() {
  for (int i = 0; i < rec->grid.height; ++i) {
    for (int j = 0; j < rec->grid.width; ++j) {
      VideoFile* currentVid = &rec->grid.grid[i][j];
      if (currentVid->id != 0) {
        QString ip = getClientByHostname(currentVid->hostname)->ClientIP;
        QString ftpurl = "ftp://" + ip + currentVid->filepath;
        QString filename = currentVid->filepath;
        filename.replace("/", "");
        filename = currentVid->hostname + "-" + filename;

        QDir savedir = QDir::currentPath();
        QString recordingTime = rec->datetime.toString("yyyy_MM_dd_hh_mm_ss");
        savedir.cd("recordings");
        savedir.cd(recordingTime);
        QString finalPath = savedir.absoluteFilePath(filename);
        qDebug() << ip << "    " << currentVid->filepath;
        FtpDownloader* download =
            new FtpDownloader(this, QUrl(ftpurl), finalPath);
        download->startDownload();
        // Update recording-data
        //.off-file now points to the videofiles on the server instead of the
        //remote-files on the clients.
        currentVid->filepath = recordingTime + "/" + filename;
        currentVid->hostname = getHostname();
        currentVid->isRemote = false;
        qDebug() << "filepath:" << currentVid->filepath
                 << "   hostname:" << currentVid->hostname;
        rec->grid.grid[i][j] = *currentVid;
      }
    }
  }
}

QString Server::getHostname() {
  QFile file("/etc/hostname");
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }
  QTextStream in(&file);
  QString line = in.readLine();
  file.close();
  return line;
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
