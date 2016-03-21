#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "../../client/src/client.h"
#include "../../server/src/server.h"
#include "../../server/src/serverthread.h"

/**
 * @brief The Communication class test some communication features between the
 * clients and the server.
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

  /**
   * @brief
   * Checks if the server gets any information about its connected clients.
   */
  void serverGetInfo();

 private:
  Server *server;
  Client *client1, *client2;
};

Communication::Communication() {}

void Communication::initTestCase() {}

void Communication::cleanupTestCase() {
  delete client1;
  delete client2;
  delete server;
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
}

QTEST_MAIN(Communication)

#include "tst_communication.moc"
