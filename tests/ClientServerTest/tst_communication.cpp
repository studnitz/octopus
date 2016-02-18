#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../client/src/client.h"
#include "../../server/src/server.h"
#include "../../server/src/serverthread.h"

class Communication : public QObject {
  Q_OBJECT

 public:
  Communication();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();

  void OneClientConnected();
  void TwoClientConnected();
  void ServerGetInfo();
  void FindServer();
};

Communication::Communication() {}

void Communication::initTestCase() {}

void Communication::cleanupTestCase() {}

/*
 * 1 Client connect and stay connected
 */
void Communication::OneClientConnected() {
  Server server;
  server.startServer();

  Client client1;
  client1.start();
  while (server.getNumClients() == 0) {
    QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients() == 1);
}
/*
 * 2 Clients connect and stay connected
 *
 *
 */
void Communication::TwoClientConnected() {
  Server server;
  server.startServer();
  Client client1;
  client1.start();
  while (server.getNumClients() == 0) {
    QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients() == 1);
  Client client2;
  client2.start();
  while (server.getNumClients() == 1) {
    QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client2.getState());
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients() == 2);
}

void Communication::ServerGetInfo() {
  Server server;
  Client client1, client2;
  server.startServer();
  client1.start();
  client2.start();
  while (server.getNumClients() != 2) {
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

void Communication::FindServer() {
  Client client;
  QVERIFY(client.findServer() == QHostAddress("127.0.0.1"));
}

QTEST_MAIN(Communication)

#include "tst_communication.moc"
