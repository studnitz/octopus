#include <QString>
#include <QtTest>
#include <QCoreApplication>

class ServerBefehle : public QObject {
  Q_OBJECT

 public:
  ServerBefehle();

 private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void testCase1();
};

ServerBefehle::ServerBefehle() {}

void ServerBefehle::initTestCase() {}

void ServerBefehle::cleanupTestCase() {}

void ServerBefehle::testCase1() { QVERIFY2(true, "Failure"); }

QTEST_MAIN(ServerBefehle)

#include "tst_serverbefehle.moc"
