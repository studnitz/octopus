#include "client.h"
#include <QStorageInfo>
#include <QProcess>
#include <QStringList>
#include <QTime>
#include <QCoreApplication>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&socket, SIGNAL(connected()), this, SLOT(getState()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
  connect(&socket, SIGNAL(readyRead()), this, SLOT(getCommand()));
}

int const Client::EXIT_CODE_REBOOT = 1337;

Client::~Client() {
  qDebug() << "Destroy Client";
  socket.close();
}
QStringList Client::listAllDevices() {
  QProcess *v4l2 = new QProcess();
  QStringList args;
  args << "--list-devices";
  v4l2->start("v4l2-ctl", args);

  if (!v4l2->waitForFinished()) return QStringList();

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
    // qDebug() << "Client not connected";
  }
}

void Client::reboot() {
  QCoreApplication *app = qobject_cast<QCoreApplication *>(this->parent());
  qDebug() << "Rebooting now...";
  app->exit(EXIT_CODE_REBOOT);
}

QString Client::currentTime() {
  QTime current = QTime::currentTime();
  QString result = current.toString();

  return result;
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

bool Client::deleteLastRecording()
{
  qDebug() << "Client: removingLastRecording" << lastRecordingPath;
  return QFile::remove(lastRecordingPath);
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
  json["IP"] = socket.localAddress().toString();
  json["Name"] = getHostname();
  json["CPU"] = getCpuUsage();
  json["Memory"] = getMemoryUsage();
  json["Disk"] = getDiskUsage();
  json["Time"] = currentTime();
  QJsonArray devices = QJsonArray::fromStringList(listAllDevices());
  json["Devices"] = devices;
  return json;
}

void Client::executeCommand(QJsonObject json) {
  if (!json.isEmpty()) {
      QString command = json["cmd"].toString();
    if (command == "getInfo") {
      // do getInfo
      QJsonObject data = getJsonInfo();
      sendData(json["cmd"].toString(), data);
      return;
    } else if (command == "recordLocally") {

      quint16 height, width, fps;

      QJsonObject settings = json["data"].toObject();
      height = settings["height"].toInt();
      width = settings["width"].toInt();
      fps = settings["fps"].toInt();

      recorder = new GstRecorder(width, height, fps);

      QString filename = recorder->recordLocally();

      lastRecordingPath = filename;
      QJsonObject data;
      data["Filename"] = filename;
      sendData(json["cmd"].toString(), data);
    } else if (command == "stopCameras") {
      qDebug() << "Client executeCommand(): stopRecording";
      recorder->stopRecording();
      connect(recorder, &GstRecorder::pipelineCleared, &GstRecorder::deleteLater);
      connect(recorder, &GstRecorder::pipelineCleared, this, &Client::videoFinishedAndWritten);
    } else if (command == "reboot") {
      reboot();
    } else if (command == "removeLastRecording") {
      QJsonObject data;
      data["fileRemoved"] = false;
      if (deleteLastRecording()) {
        data["fileRemoved"] = true;
        sendData(json["cmd"].toString(), data);
      }
    }
  }
}

QTcpSocket::SocketState Client::getState() const { return socket.state(); }

void Client::videoFinishedAndWritten()
{
  QJsonObject data;
  data["filename"] = lastRecordingPath;
  qDebug() << "Client videoFinishedAndWritten(): " << lastRecordingPath;
  sendData("videoReady", data);
}

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
