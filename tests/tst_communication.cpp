#include <QString>
#include <QtTest>
#include <QtCore>
//#include <QCoreApplication>
#include "client.h"
#include "server.h"
#include "serverthread.h"
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
  Server server;
  server.startServer();

  Client client1;
  client1.start(1234);
  while (server.getNumClients() ==0 ) {
        QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients()==1);

}
/*
 * 2 Clients connect and stay connected
 *
 *
 */
 void communication::testCase2() {
  Server server;
  server.startServer();
  Client client1;
  client1.start(1234);
  while (server.getNumClients() ==0 ) {
        QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients()==1);
  Client client2;
  client2.start(1234);
  while (server.getNumClients() ==1 ) {
        QTest::qWait(200);
  }
  QVERIFY(QAbstractSocket::ConnectedState == client2.getState());
  QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
  QVERIFY(server.getNumClients()==2);

  QList<ServerThread*> clients = server.getClients();

  while (clients.first()->ClientInfo[0] ==0 ) {
        QTest::qWait(200);
  }

  qDebug() << "INFO :"<< clients.first()->ClientInfo[0];
}

 void communication::testCase3() {


}

 void communication::testCase4() {}

QTEST_MAIN(communication)

#include "tst_communication.moc"
