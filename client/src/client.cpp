#include "client.h"
#include <QStorageInfo>
#include <QProcess>
#include <QStringList>

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
QStringList Client::listAllDevices() {
  QProcess* v4l2 = new QProcess();
  QStringList args;
  args << "--list-devices";
  v4l2->start("v4l2-ctl", args);

  if(!v4l2->waitForFinished()) return QStringList();

  QString output(v4l2->readAllStandardOutput());
  QStringList outputList = output.split("\n");
  outputList = outputList.filter("dev");
  outputList.replaceInStrings("\t", "");

  return outputList;
}


void Client::start(QString ip, quint16 port) {
  QHostAddress serverIp = QHostAddress(ip);

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
  while (!socket.atEnd()) {
    // Read Json object from socket
    ba = socket.readLine();
    json = QJsonDocument::fromJson(ba).object();

    executeCommand(json);
  }
}

void Client::sendData(QString cmd, QJsonObject &str) {
  if (socket.state() == QTcpSocket::ConnectedState) {
    QByteArray msg;
    QJsonObject json = QJsonObject();

    json["cmd"] = cmd;
    json["data"] = str;
    msg = QJsonDocument(json).toJson(QJsonDocument::Compact).append("\n");
    socket.write(msg);
  }
}

QJsonObject Client::getJsonInfo() {
  QJsonObject json;
  json["IP"] = socket.peerAddress().toString();
  json["Name"] = getHostname();
  json["CPU"] = getCpuUsage();
  json["Memory"] = getMemoryUsage();
  json["Disk"] = getDiskUsage();
  QJsonArray devices = QJsonArray::fromStringList(listAllDevices());
  json["Devices"] = devices;
  return json;
}

void Client::executeCommand(QJsonObject json) {
  if (!json.isEmpty()) {
    if (json["cmd"].toString().compare("getInfo") == 0) {
      // do getInfo
      QJsonObject data = getJsonInfo();
      sendData(json["cmd"].toString(), data);
      return;
    } else if (json["cmd"].toString().compare("recordLocally") == 0) {
      recorder.recordLocally();
      isRecording = true;
    } else if (json["cmd"].toString().compare("stopCameras") == 0) {
      if (isRecording) {
        isRecording = false;
        recorder.stopRecording();
      }
    }
  }
}


QTcpSocket::SocketState Client::getState() const { return socket.state(); }

QString Client::getHostname() {
  QFile file("/etc/hostname");
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }
  QTextStream in(&file);
  QString line = in.readLine();
  file.close();
  return line;
}

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
