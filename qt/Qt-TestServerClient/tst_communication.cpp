#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include "client.h"
#include "myserver.h"


class communication : public QObject
{
    Q_OBJECT

public:
    communication();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
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

void communication::testCase1()
{
    MyServer server;
    server.startServer();
    Client client1;
    client1.start("127.0.0.1", 1234);
    Client client2;
    client2.start("127.0.0.1", 1234);
}

QTEST_APPLESS_MAIN(communication)

#include "tst_communication.moc"
