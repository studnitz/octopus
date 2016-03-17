#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "../server/src/server.h"
#include "../server/src/serverinterface.h"
#include "../server/src/recording.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class ServerBefehle : public QObject {
  Q_OBJECT

 public:
  ServerBefehle();

 private Q_SLOTS:
  /**
   * @brief
   * Initialize the server
   */
  void initTestCase();

  void cleanupTestCase();

  /**
   * @brief
   * Test proper creation of VideFiles, their insertion to the grid, the grids
   * insertion into a recording, as well as the proper conversion of a recording
   * to JSON and back to a recording again.
   */
  void testCase1();

 private:
  Server *server;
  ServerInterface *serverInterface;
};

ServerBefehle::ServerBefehle() {}

void ServerBefehle::initTestCase() {
  server = new Server(this);
  server->startServer();

  serverInterface = new ServerInterface(this);
  serverInterface->setServer(server);
  serverInterface->start();

  QVERIFY(server != NULL);
  QVERIFY(serverInterface != NULL);
}

void ServerBefehle::cleanupTestCase() {
  delete server;
  delete serverInterface;
  server = NULL;
  serverInterface = NULL;

  QVERIFY(server == NULL);
  QVERIFY(serverInterface == NULL);
}

void ServerBefehle::testCase1() {
  QDate date(2016, 03, 31);
  QTime time(10, 30, 00);
  Grid *grid = new Grid(2, 2);
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

  QJsonObject json = QJsonObject();

  rec->write(json);
  Recording rec2;
  rec2.read(json);

  for (int i = 0; i < rec->grid.grid.size(); i++)
    for (int j = 0; j < rec->grid.grid[i].size(); j++)
      QVERIFY(rec->grid.grid[i][j].id == rec->grid.grid[i][j].id);

  delete rec;
  delete grid;
  delete vid1;
  delete vid2;
  delete vid3;
  delete vid4;
}

QTEST_MAIN(ServerBefehle)

#include "tst_serverbefehle.moc"
