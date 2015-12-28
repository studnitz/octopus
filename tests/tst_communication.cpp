#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "client.h"
#include "myserver.h"
#include "mythread.h"
#include <QTime>
#include <QThread>

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
  void testCase4();
};

communication::communication() {}

void communication::initTestCase() {}

void communication::cleanupTestCase() {}

/*
 * 1 Client connect and stay connected
 *
 *
 */

void communication::testCase1() {
  MyServer* server = new MyServer();
  server->startServer();
  qDebug() << "Server started";
  Client client1;
  client1.start(1234);
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  delete server;
}
/*
 * 2 Clients connect and stay connected
 *
 *
 */
void communication::testCase2() {
  MyServer server;
  server.startServer();
  Client client1;
  client1.start(1234);
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  Client client2;
  client2.start(1234);
  QVERIFY(QAbstractSocket::ConnectedState == client2.getState());
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
}

void communication::testCase3() {
  MyServer server;
  server.startServer(); 

  Client client1;
  client1.start(1234);
  Client client2;
  client2.start(1234);
  QVERIFY(QAbstractSocket::ConnectedState == client2.getState());
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QTest::qSleep(10000);
  qDebug() << server.getClients().size();
  QVERIFY(server.getClients().size() == 2);
}

void communication::testCase4() {}

QTEST_APPLESS_MAIN(communication)

#include "tst_communication.moc"
