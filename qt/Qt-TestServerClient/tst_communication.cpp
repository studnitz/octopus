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
    Client client2;
    client2.start("127.0.0.1", 1234);
    client2.startTransfer();
    QTest::qSleep(10000);
    qDebug() << client2.getState();
}

QTEST_APPLESS_MAIN(communication)

#include "tst_communication.moc"
