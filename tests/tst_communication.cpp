#include <QtTest>
#include "client.h"
#include "server.h"
#include "serverthread.h"

class communication : public QObject {
  Q_OBJECT

 public:
  communication();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void testCase1();
  void testCase2();
  void testCase3();
};

communication::communication() {}

void communication::initTestCase() {}

void communication::cleanupTestCase() {}

/*
 * 1 Client connect and stay connected
 */
void communication::testCase1() {
  Server server;
  server.startServer();

  Client client1;
  client1.start(1234);
  while (server.getNumClients() == 0) {
    QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients() == 1);
}
/*
 * 2 Clients connect and stay connected
 */
void communication::testCase2() {
  Server server;
  server.startServer();
  Client client1;
  client1.start(1234);
  while (server.getNumClients() == 0) {
    QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients() == 1);
  Client client2;
  client2.start(1234);
  while (server.getNumClients() == 1) {
    QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client2.getState());
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients() == 2);

  QList<ServerThread*> clients = server.getClients();

  while (clients.first()->ClientInfo[0] == 0) {
    QTest::qWait(200);
  }
}

void communication::testCase3() {
  Server server;
  server.startServer();
  Client client1;
  client1.start(1234);
  while (server.getNumClients() == 0) {
    QTest::qWait(200);
  }
  Client client2;
  client2.start(1234);
  while (server.getNumClients() == 1) {
    QTest::qWait(200);
  }
  QList<ServerThread*> clients = server.getClients();

  while (clients.first()->ClientInfo[0] == 0) {
    QTest::qWait(200);
  }
  while (clients.at(1)->ClientInfo[0] == 0) {
    QTest::qWait(200);
  }

  QVERIFY(clients.at(0)->ClientInfo[0] != 0);
  QVERIFY(clients.at(1)->ClientInfo[0] != 0);
}

QTEST_MAIN(communication)

#include "tst_communication.moc"
