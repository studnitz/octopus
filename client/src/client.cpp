#include "client.h"
#include <QStorageInfo>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&socket, SIGNAL(connected()), this, SLOT(getState()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
  connect(&socket, SIGNAL(readyRead()), this, SLOT(getCommand()));

  // hostfound() signal connect?
}

Client::~Client() {
  qDebug() << "Destroy Client";
  socket.close();
}

void Client::sendInfo() {
  QByteArray message;
  message.setNum(0).append("\n");
  socket.write(message);
  message.setNum((float)getMemoryUsage()).append("\n");
  socket.write(message);
  message.setNum((float)getCpuUsage()).append("\n");
  socket.write(message);
  message.setNum((float)getDiskUsage()).append("\n");
  socket.write(message);
  message.setNum((float)getFreeMemory()).append("\n");
  socket.write(message);
  message.setNum((float)getAllMemory()).append("\n");
  socket.write(message);
  message.setNum((float)getFreeDisk()).append("\n");
  socket.write(message);
  message.setNum((float)getTotalDisk()).append("\n");
  socket.write(message);
}

void Client::start(QString ip, quint16 port) {
  findCamera();
  QHostAddress serverIp;
  if (ip == "127.0.0.1") {
    qDebug() << "Searching Server";
    serverIp = findServer();
  } else {
    qDebug() << "Connecting to" << ip;
    serverIp = QHostAddress(ip);
  }

  syncTime();

  if (timesync) {
    //  qDebug() << "Time synced";
  } else {
    // qDebug() << "Time Server not avaivble";
  }

  socket.connectToHost(serverIp, port);

  if (socket.waitForConnected()) {  // Timeout included in waitfor
    qDebug() << "Client connected";
  } else {
    qDebug() << "Client not connected";
  }
}

void Client::getCommand() {
  QByteArray ba;
  QJsonObject json;
  while (!socket->atEnd()) {
    // Read Json object from socket
    ba = socket->readLine();
    json = QJsonDocument::fromJson(ba).object();

    executeCommand(json);
  }
}

void Client::executeCommand(QJsonObject json) {
  if (!json.isEmpty()) {
    if (json["cmd"].toString().compare("getInfo") == 0) {
      // do getInfo
      QJsonObject data = getJsonInfo();
      server->broadcastCommand(0);
      sendData(json["cmd"].toString(), data);
      return;
    } else if (json["cmd"].toString().compare("recordLocally")==0) {
      //todo record
    }else if (json["cmd"].toString().compare("stopCameras")==0){
      //todo stop
    }
  }
  QJsonObject data;
  data["data"] = "lololol";
  sendData("TEST", data);
}

std::string Client::isConnected() { return "yes"; }

QTcpSocket::SocketState Client::getState() const { return socket.state(); }

QHostAddress Client::findServer() {
  QHostAddress serverIP;
  QNetworkInterface iface;
  QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
  QListIterator<QNetworkInterface> it(interfaces);
  while (it.hasNext()) {
    iface = it.next();
    if (iface.humanReadableName() == "eth0") {
      QList<QNetworkAddressEntry> entries = iface.addressEntries();
      if (!entries.isEmpty()) {
        QNetworkAddressEntry entry = entries.first();
        serverIP = entry.broadcast();
        return serverIP;
      } else {
        qDebug() << "No LAN Cable connected";
        break;
      }
    }
  }
  qDebug() << "Connecting to localhost";
  serverIP = QHostAddress("127.0.0.1");
  return serverIP;
}

void Client::syncTime() {
  // HARDCODED
  timesync = true;
  return;
}

void Client::findCamera() {
  // qDebug() << "Camera found";
  return;
};

double Client::getCpuUsage() {
  double percent;
  FILE *file;
  unsigned int totalUser, totalUserLow, totalSys, totalIdle, total;

  file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %d %d %d %d", &totalUser, &totalUserLow, &totalSys,
         &totalIdle);
  fclose(file);

  total = (totalUser) + (totalUserLow) + (totalSys);
  percent = total;
  total += (totalIdle);
  percent /= total;
  percent *= 100;

  return percent;
}

int Client::getAllMemory() {
  QFile file("/proc/meminfo");
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }

  QTextStream in(&file);
  QString line = in.readLine();

  QRegExp rx("[ ]");
  QStringList list = line.split(rx, QString::SkipEmptyParts);

  file.close();
  return atol(list.at(1).toStdString().c_str());
}

int Client::getFreeMemory() {
  QFile file("/proc/meminfo");
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }

  QTextStream in(&file);
  QString line = in.readLine();
  line = in.readLine();
  line = in.readLine();
  QRegExp rx("[ ]");
  QStringList list = line.split(rx, QString::SkipEmptyParts);

  file.close();
  return atol(list.at(1).toStdString().c_str());
}

int Client::getFreeDisk() {
  QStorageInfo info("/");
  return info.bytesAvailable() / 1024;
}

double Client::getDiskUsage() {
  return 100 - (getFreeDisk() / (float)getTotalDisk()) * 100;
}

float Client::getMemoryUsage() {
  int free_mem = getFreeMemory();
  int total_mem = getAllMemory();
  return 100 - (free_mem / ((float)total_mem)) * 100;
}

int Client::getTotalDisk() {
  QStorageInfo info("/");
  return info.bytesTotal() / 1024;
}
