#include <QtTest>

class MyTest : public QObject
{
    Q_OBJECT
private slots:
    void myTest();
};

void MyTest::myTest()
{
    QCOMPARE(true, true);
}

QTEST_MAIN(MyTest)
#include "test.moc"
