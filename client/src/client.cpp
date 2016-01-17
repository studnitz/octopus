#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&socket, SIGNAL(connected()), this, SLOT(sendInfo()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
  connect(&socket, SIGNAL(readyRead()), this, SLOT(waitForCommand()));

  // hostfound() signal connect?
}

Client::~Client() {
  qDebug() << "Destroy Client";
  socket.close();
}

void Client::sendInfo() {}

void Client::start(quint16 port) {
  this->findCamera();

  QHostAddress addr(this->findServer());

  this->syncTime();

  if (timesync) {
    qDebug() << "Time synced";
  } else {
    qDebug() << "Time Server not avaivble";
  }

  socket.connectToHost(addr, port);

  if (socket.waitForConnected()) {  // Timeout included in waitfor
    qDebug() << "Client connected";
  } else {
    qDebug() << "Client not connected";
  }
}

void Client::waitForCommand() {
  QByteArray message;
  message = socket.readAll();
  int command = message.toInt();
  qDebug() << message << "Command recieved";

  switch (command) {
    case 0:
      qDebug() << "sending Info about Memory Usage";
      message.setNum((int)getMemoryUsage());
      break;
    case 1:
      qDebug() << "sending Info about CPU Usage";
      message.setNum((int)getCpuUsage());
      break;
    case 2:
      qDebug() << "sending Info about Disk Usage";
      message.setNum((int)getDiskUsage());
      break;
    case 3:
      qDebug() << "sending Info about Free Memory";
      message.setNum((int)getFreeMemory());
      break;
    case 4:
      qDebug() << "sending Info about All Memory";
      message.setNum((int)getAllMemory());
      break;
    case 5:
      qDebug() << "sending Info about Free Disk";
      message.setNum((int)getFreeDisk());
      break;
    case 6:
      qDebug() << "sending Info about Total Disk";
      message.setNum((int)getTotalDisk());
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    default:
      message.append("command unknown \n");
      break;
  }
  socket.write(message);
}

std::string Client::isConnected() { return "yes"; }

QTcpSocket::SocketState Client::getState() const { return socket.state(); }

QString Client::findServer() {
  /*
   * Find SERVER IP = DHCP
   * NEED TIMEOUT INCLUDED
   *
   */
  QString address = "127.0.0.1";  // HARDCODED

  return address;
}

void Client::syncTime() {
  // HARDCODED
  timesync = true;
  return;
}

void Client::findCamera() {
  qDebug() << "Camera found";
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
