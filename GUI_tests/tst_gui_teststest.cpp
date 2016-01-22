#include <QString>
#include <QtTest>
#include <QCoreApplication>

class GUI_testsTest : public QObject
{
    Q_OBJECT

public:
    GUI_testsTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

GUI_testsTest::GUI_testsTest() // Konstruktor
{
}

void GUI_testsTest::initTestCase()
{
}

void GUI_testsTest::cleanupTestCase()
{
}

void GUI_testsTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(GUI_testsTest)

#include "tst_gui_teststest.moc"
