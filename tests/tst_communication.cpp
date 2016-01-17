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
  void OneClientConnected();
  void TwoClientConnected();
  void ServerGetInfo();
  void FindServer();
};

communication::communication() {}

void communication::initTestCase() {}

void communication::cleanupTestCase() {}

/*
 * 1 Client connect and stay connected
 */
void communication::OneClientConnected() {
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
 */
void communication::TwoClientConnected() {
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

void communication::ServerGetInfo() {
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

void communication::FindServer(){
    Client client;
    QVERIFY(client.findServer()==QHostAddress("127.0.0.1"));
}

QTEST_MAIN(communication)

#include "tst_communication.moc"
