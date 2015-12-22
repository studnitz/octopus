#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "../client/src/client.h"
#include "../server/src/myserver.h"


class communication : public QObject
{
    Q_OBJECT

public:
    communication();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase2();
};

communication::communication()
{

}

void communication::initTestCase()
{

}

void communication::cleanupTestCase()
{

}

/*
 * 1 Client connect and stay connected
 *
 *
 */

void communication::testCase1()
{
    MyServer* server = new MyServer;
    server->startServer();
    qDebug() << "Server started";
    Client client1;
     client1.start("127.0.0.1", 1234);
     QVERIFY(QAbstractSocket::ConnectedState == client1.getState());


}
/*
 * 2 Clients connect and stay connected
 *
 *
 */
void communication::testCase2()
{

    MyServer* server = new MyServer;
    server->startServer();
    qDebug() << "Server started";
    Client client1;
    client1.start("127.0.0.1", 1234);
    QVERIFY(QAbstractSocket::ConnectedState == client1.getState());
    Client client2;
    client2.start("127.0.0.1", 1234);
    QVERIFY(QAbstractSocket::ConnectedState == client2.getState());
    QVERIFY(QAbstractSocket::ConnectedState == client1.getState());

}

QTEST_APPLESS_MAIN(communication)

#include "tst_communication.moc"
