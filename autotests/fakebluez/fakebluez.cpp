#include "fakebluez.h"
#include "testinterface.h"

#include <QDBusConnection>

FakeBluez::FakeBluez(QObject *parent)
    : QObject(parent)
{
    m_testInterface = new TestInterface(this);
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/"),
                                                m_testInterface,
                                                QDBusConnection::ExportAllContents);
}

void FakeBluez::runTest(const QString &testName)
{
    if (testName == QLatin1String("bluez-not-exporting-interfaces")) {
        // We do nothing here
    }
}
