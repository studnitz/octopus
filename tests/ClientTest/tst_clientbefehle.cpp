#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "src/client.h"

/**
 * @brief The ClientBefehle class tests some standalone features of the client.
 * It does not test any communication between the clients and the server.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence.
 * @date 31.03.2016
 */
class ClientBefehle : public QObject {
  Q_OBJECT

 public:
  ClientBefehle();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  /**
   * @brief
   * Check if the value of the disk usage is correct (recalculate the value
   * and compare it to the given value)
   */
  void DiskUsageCorrectness();

  /**
   * @brief
   * Check that the disk usage never goes out of the range (0% <= range <= 100%)
   */
  void DiskUsageBorders();

  /**
   * @brief
   * Check if the value of the memory usage is correct (recalculate the value
   * and compare it to the given value)
   */
  void MemUsageCorrectness();

  /**
   * @brief
   * Check that the memory usage never goes out of the range (0% <= range <=
   * 100%)
   */
  void MemUsageBorders();

  /**
   * @brief
   * Check that the CPU usage never goes out of the range (0% <= range <= 100%)
   */
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
