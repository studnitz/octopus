#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <client.h>

class ClientBefehlTestsTest : public QObject {
  Q_OBJECT

 public:
  ClientBefehlTestsTest();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void testCase1_data();
  void testCase1();
  void DiskUsageCorrectness();
  void DiskUsageBorders();
  void MemUsageCorrectness();
  void MemUsageBorders();
  void CPUUsageBorders();
};

ClientBefehlTestsTest::ClientBefehlTestsTest() {
  DiskUsageCorrectness();
  DiskUsageBorders();
  MemUsageCorrectness();
  MemUsageBorders();
  CPUUsageBorders();
}

void ClientBefehlTestsTest::DiskUsageCorrectness() {
  Client* client = new Client();
  int Free = client->getFreeDisk();
  int Total = client->getTotalDisk();
  float Usage = 100 - (Free / (float)Total) * 100;
  QVERIFY(Usage <= client->getDiskUsage() + 1 &&
          Usage >= client->getDiskUsage() - 1);
}

void ClientBefehlTestsTest::DiskUsageBorders() {
  Client* client = new Client();
  QVERIFY(client->getDiskUsage() > 0 && client->getDiskUsage() < 100);
}

void ClientBefehlTestsTest::MemUsageCorrectness() {
  Client* client = new Client();
  int freeMem = client->getFreeMemory();
  int totMem = client->getAllMemory();
  float Usage = (100 - freeMem / (float)totMem * 100);
  QVERIFY(Usage <= client->getMemoryUsage() + 1 &&
          Usage >= client->getMemoryUsage() - 1);
}
void ClientBefehlTestsTest::MemUsageBorders() {
  Client* client = new Client();
  QVERIFY(client->getMemoryUsage() > 0 && client->getMemoryUsage() < 100);
}

void ClientBefehlTestsTest::CPUUsageBorders() {
  Client* client = new Client();
  QVERIFY(client->getCpuUsage() > 0 && client->getCpuUsage() < 100);
}
void ClientBefehlTestsTest::initTestCase() {}

void ClientBefehlTestsTest::cleanupTestCase() {}

void ClientBefehlTestsTest::testCase1_data() {
  QTest::addColumn<QString>("data");
  QTest::newRow("0") << QString();
}

void ClientBefehlTestsTest::testCase1() {
  QFETCH(QString, data);
  QVERIFY2(true, "Failure");
}

QTEST_MAIN(ClientBefehlTestsTest)

#include "tst_clientbefehlteststest.moc"
