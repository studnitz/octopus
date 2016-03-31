#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "src/client.h"
#include "src/server.h"
#include "src/serverthread.h"
#include "src/serverinterface.h"

/**
 * @brief The Communication class is a composite test class made out of three
 * test classes (ServerBefehle, Communication and ClientBefehle).
 * The ServerBefehle class tests some standalone features of the server.
 * This means features that do not need any clients or UIs to interact with.
 * The Communication class test some communication features between the
 * clients and the server.
 * The ClientBefehle class tests some standalone features of the client.
 * It does not test any communication between the clients and the server.
 * @author Bartosz Milejski, Yannick Schädele, Nicolas Schickert, Alexander von
 * Studnitz
 * @copyright GNU Public Licence
 * @date 31.03.2016
 */
class Communication : public QObject {
  Q_OBJECT

 public:
  Communication();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  //----- START SERVER TESTS -----

  /**
   * @brief testCase1 is a little-bit-too-big Test Case for pretty much
   * everything. It tests proper creation of VideFiles, their insertion to the
   * grid, the grids insertion into a recording, as well as the proper
   * conversion of a recording to JSON and back to a recording again.
   */
  void serverTestCase();

  //----- START CLIENT-SERVER TESTS -----

  /**
   * @brief
   * Checks if server can accept one client at a time.
   */
  void oneClientConnected();

  /**
   * @brief
   * Checks if server can handle multiple (2) connections at a time.
   */
  void twoClientConnected();

  //----- START CLIENT TESTS -----

  /**
   * @brief
   * Checks if the server gets any information about its connected clients.
   */
  void serverGetInfo();

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

 private:
  Server *server;
  ServerInterface *serverInterface;
  Client *client1, *client2;
};

Communication::Communication() {}

void Communication::initTestCase() {}

void Communication::cleanupTestCase() {}

void Communication::serverTestCase() {
  // Test proper object creation
  server = new Server(this);
  server->startServer();
  serverInterface = new ServerInterface(this);
  serverInterface->setServer(server);
  serverInterface->start();

  QVERIFY(server != NULL);
  QVERIFY(serverInterface != NULL);

  /*
   * Test proper:
   *  - Creation of Recording, Grid, VideoFiles
   *  - Addition of VideoFiles to a Grid
   *  - Looking up VideoFiles by their ID
   *  - Looking up a VideoFile's position by it's ID
   */
  QDate date(2016, 03, 31);
  QTime time(10, 30, 00);
  Grid *grid = new Grid(2, 2);
  grid->resizeGrid(2, 2);
  Recording *rec = new Recording(QDateTime(date, time), *grid);
  QString path1("\testvideofile.mp4");
  QString name1("CLIENT1");
  QString path2("\asdf.mov");
  QString name2("OFFCLIENT");
  QString devicepath1("/dev/video0");
  QString devicepath2("/dev/video1");

  VideoFile *vid1 = new VideoFile(0);
  VideoFile *vid2 = new VideoFile(1337, false, path1, name1, devicepath1);
  VideoFile *vid3 = new VideoFile(1338, true, path2, name2, devicepath2);
  VideoFile *vid4 = new VideoFile(0);

  rec->grid.addSource(*vid1, 0, 0);
  rec->grid.addSource(*vid2, 0, 1);
  rec->grid.addSource(*vid3, 1, 0);
  rec->grid.addSource(*vid4, 1, 1);

  QVERIFY(&rec->grid.grid.at(1).at(0) == &rec->grid.getVideoFileById(1337));
  QVERIFY(&rec->grid.grid[0][1] == &rec->grid.getVideoFileById(1338));
  QVERIFY(rec->grid.getVideoFilePositionById(1337).first == 1 &&
          rec->grid.getVideoFilePositionById(1337).second == 0);

  // Test proper deletion of VideoFiles
  rec->grid.deleteSource(0, 1);
  QVERIFY(rec->grid.grid[1][0].id == 0);

  // Test proper size-function of Grid
  QVERIFY(rec->grid.size() == 1);

  // Test proper writing to JSON and reading from JSON again
  QJsonObject json = QJsonObject();
  rec->write(json);
  Recording *rec2 = new Recording();
  rec2->read(json);

  for (int i = 0; i < rec->grid.grid.size(); i++)
    for (int j = 0; j < rec->grid.grid[i].size(); j++)
      QVERIFY(rec->grid.grid[i][j].id == rec2->grid.grid[i][j].id);

  // Test proper writing to file and reading from file
  server->rec = rec;
  server->updateRecording();
  Recording *rec3 = new Recording();
  rec3->loadRecording(QString("recordings/") + QString("/") +
                      rec->datetime.toString("yyyy_MM_dd_hh_mm_ss") +
                      QString(".off"));
  for (int i = 0; i < rec->grid.grid.size(); i++)
    for (int j = 0; j < rec->grid.grid[i].size(); j++)
      QVERIFY(rec->grid.grid[i][j].id == rec3->grid.grid[i][j].id);

  // Test proper getHostname-function of server
  QString hostname = server->getHostname();
  QVERIFY(hostname.length() > 0);

  // Cleanup
  QDir dir = QDir::current();
  dir.cd("recordings");
  dir.removeRecursively();
  delete rec;
  delete rec2;
  delete rec3;
  delete grid;
  delete vid1;
  delete vid2;
  delete vid3;
  delete vid4;
  delete server;
  delete serverInterface;
  server = NULL;
  serverInterface = NULL;

  // Test proper cleanup
  QVERIFY(server == NULL);
  QVERIFY(serverInterface == NULL);
}

void Communication::oneClientConnected() {
  server = new Server();
  server->startServer();

  client1 = new Client();
  client1->start();

  int numClients = 0;
  while (numClients == 0) {
    numClients = server->getNumClients();
    QTest::qWait(200);
  }

  // Test if it's possible for one client to connect
  QVERIFY(QAbstractSocket::ConnectedState == client1->getState());
  QVERIFY(server->getNumClients() == 1);
  delete client1;
  delete server;
}

void Communication::twoClientConnected() {
  server = new Server();
  server->startServer();

  client1 = new Client();
  client1->start();

  while (server->getNumClients() == 0) {
    QTest::qWait(200);
  }

  QVERIFY(QAbstractSocket::ConnectedState == client1->getState());
  QVERIFY(server->getNumClients() == 1);

  client2 = new Client();
  client2->start();
  while (server->getNumClients() == 1) {
    QTest::qWait(200);
  }

  QVERIFY(QAbstractSocket::ConnectedState == client2->getState());
  QVERIFY(QAbstractSocket::ConnectedState == client1->getState());
  QVERIFY(server->getNumClients() == 2);
  delete client1;
  delete client2;
  delete server;
}

void Communication::serverGetInfo() {
  server = new Server();
  server->startServer();

  client1 = new Client();
  client1->start();
  client2 = new Client();
  client2->start();

  while (server->getNumClients() != 2) {
    QTest::qWait(200);
  }

  QList<ServerThread *> *clients =
      new QList<ServerThread *>(server->getClients());

  while (clients->length() != 2) {
    QTest::qWait(200);
  }

  QVERIFY(clients->length() == 2);
  QVERIFY(clients->at(0)->clientName != NULL);
  QVERIFY(clients->at(1)->clientName != NULL);

  delete client1;
  delete client2;
  delete server;
}

void Communication::DiskUsageCorrectness() {
  client1 = new Client();
  int Free = client1->getFreeDisk();
  int Total = client1->getTotalDisk();
  float Usage = 100 - (Free / (float)Total) * 100;
  QVERIFY(Usage <= client1->getDiskUsage() + 1 &&
          Usage >= client1->getDiskUsage() - 1);
  delete client1;
}

void Communication::DiskUsageBorders() {
  client1 = new Client();
  QVERIFY(client1->getDiskUsage() > 0 && client1->getDiskUsage() < 100);
  delete client1;
}

void Communication::MemUsageCorrectness() {
  client1 = new Client();
  int freeMem = client1->getFreeMemory();
  int totMem = client1->getAllMemory();
  float Usage = (100 - freeMem / (float)totMem * 100);
  QVERIFY(Usage <= client1->getMemoryUsage() + 1 &&
          Usage >= client1->getMemoryUsage() - 1);
  delete client1;
}
void Communication::MemUsageBorders() {
  client1 = new Client();
  QVERIFY(client1->getMemoryUsage() > 0 && client1->getMemoryUsage() < 100);
  delete client1;
}

void Communication::CPUUsageBorders() {
  client1 = new Client();
  QVERIFY(client1->getCpuUsage() > 0 && client1->getCpuUsage() < 100);
  delete client1;
}

QTEST_MAIN(Communication)

#include "tst_communication.moc"
