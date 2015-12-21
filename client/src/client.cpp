#include "client.h"
#include <QHostAddress>

Client::Client(QObject *parent) : QObject(parent) {
  if (!this->connect(&client, SIGNAL(connected()), this,
                     SLOT(startTransfer()))) {
    qDebug() << "Could not start client";
  } else {
    qDebug() << "Client started";
  }
}

Client::~Client() { client.close(); }


/**
 * @brief Client::start starts a connection
 * @param address IP-Address of the server
 * @param port  Port of the server
 */
void Client::start(QString address, quint16 port) {
  QHostAddress addr(address);
  client.connectToHost(addr, port);
}

/**
 * @brief Client::startTransfer sends a message to the server
 */
void Client::startTransfer() { client.write("Ready for Command\n", 18); }


/**
 * @brief Client::getCpuUsage
 * @return current CPU-Usage in percent
 */
double Client::getCpuUsage() {
  double percent;
  FILE *file;
  unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

  file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys,
         &totalIdle);
  fclose(file);

  total = (totalUser) + (totalUserLow) + (totalSys);
  percent = total;
  total += (totalIdle);
  percent /= total;
  percent *= 100;

  return percent;
}

/**
 * @brief Client::getAllMemory
 * @return total memory in KB
 */
long Client::getAllMemory() {
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

/**
 * @brief Client::getFreeMemory
 * @return free memory in KB
 */
long Client::getFreeMemory() {
  QFile file("/proc/meminfo");
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << file.errorString();
  }

  QTextStream in(&file);
  QString line = in.readLine();
  line = in.readLine();

  QRegExp rx("[ ]");
  QStringList list = line.split(rx, QString::SkipEmptyParts);

  file.close();
  return atol(list.at(1).toStdString().c_str());
}


/**
 * @brief Client::getFreeDisk
 * @return free disk space in Bytes
 */
ulong Client::getFreeDisk() {
  QStorageInfo info("/");
  return info.bytesAvailable();
}

/**
 * @brief Client::getDiskUsage
 * @return disk usage in percent
 */
double Client::getDiskUsage() {
  QStorageInfo info("/");
  return 100 - (getFreeDisk() / (float)getTotalDisk()) * 100;
}


/**
 * @brief Client::getTotalDisk
 * @return total disk space in Bytes
 */
ulong Client::getTotalDisk() {
  QStorageInfo info("/");
  return info.bytesTotal();
}

/**
 * @brief Client::isConnected is used to test the connection
 * @return "yes"
 */
std::string Client::isConnected() { return "yes"; }
