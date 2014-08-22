#include "testinterface.h"
#include "fakebluez.h"

#include <QTimer>

TestInterface::TestInterface(FakeBluez *parent)
    : QDBusAbstractAdaptor(parent)
    , m_fakeBluez(parent)
{
}

void TestInterface::emitActionFinished()
{
    Q_EMIT actionFinished();
}

void TestInterface::runTest(const QString &testName)
{
    m_fakeBluez->runTest(testName);
}

void TestInterface::runAction(const QString &object, const QString &actionName, const QVariantMap &properties)
{
    m_fakeBluez->runAction(object, actionName, properties);
}
