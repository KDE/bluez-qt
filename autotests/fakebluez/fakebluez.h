#ifndef FAKEBLUEZ_H
#define FAKEBLUEZ_H

#include <QObject>

class TestInterface;

class FakeBluez : public QObject
{
public:
    explicit FakeBluez(QObject *parent = 0);

    void runTest(const QString &testName);

private:
    TestInterface *m_testInterface;
};

#endif // FAKEBLUEZ_H
