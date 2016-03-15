#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "src/client.h"

class ClientBefehle : public QObject {
  Q_OBJECT

 public:
  ClientBefehle();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  void DiskUsageCorrectness();
  void DiskUsageBorders();
  void MemUsageCorrectness();
  void MemUsageBorders();
  void CPUUsageBorders();
};

ClientBefehle::ClientBefehle() {}

void ClientBefehle::initTestCase() {}

void ClientBefehle::cleanupTestCase() {}

void ClientBefehle::DiskUsageCorrectness() {
  Client* client = new Client();
  int Free = client->getFreeDisk();
  int Total = client->getTotalDisk();
  float Usage = 100 - (Free / (float)Total) * 100;
  QVERIFY(Usage <= client->getDiskUsage() + 1 &&
          Usage >= client->getDiskUsage() - 1);
}

void ClientBefehle::DiskUsageBorders() {
  Client* client = new Client();
  QVERIFY(client->getDiskUsage() > 0 && client->getDiskUsage() < 100);
}

void ClientBefehle::MemUsageCorrectness() {
  Client* client = new Client();
  int freeMem = client->getFreeMemory();
  int totMem = client->getAllMemory();
  float Usage = (100 - freeMem / (float)totMem * 100);
  QVERIFY(Usage <= client->getMemoryUsage() + 1 &&
          Usage >= client->getMemoryUsage() - 1);
}
void ClientBefehle::MemUsageBorders() {
  Client* client = new Client();
  QVERIFY(client->getMemoryUsage() > 0 && client->getMemoryUsage() < 100);
}

void ClientBefehle::CPUUsageBorders() {
  Client* client = new Client();
  QVERIFY(client->getCpuUsage() > 0 && client->getCpuUsage() < 100);
}

QTEST_MAIN(ClientBefehle)

#include "tst_clientbefehle.moc"
