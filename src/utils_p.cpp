#include "utils_p.h"
#include "qbluez_export.h"

#include <QStringList>
#include <QDBusConnection>

namespace QBluez
{

class GlobalData
{
public:
    explicit GlobalData();

    bool testRun;
    QString orgFreedesktopDBus;
    QString orgBluez;
    QString orgBluezAdapter1;
    QString orgBluezDevice1;
    QString orgBluezAgentManager1;
    QString orgBluezObex;
    QString orgBluezObexClient1;
    QString orgBluezObexAgentManager1;
    QString orgBluezObexSession1;
    QString orgBluezObexTransfer1;
};

GlobalData::GlobalData()
{
    testRun = false;
    orgFreedesktopDBus = QStringLiteral("org.freedesktop.DBus");
    orgBluez = QStringLiteral("org.bluez");
    orgBluezAdapter1 = QStringLiteral("org.bluez.Adapter1");
    orgBluezDevice1 = QStringLiteral("org.bluez.Device1");
    orgBluezAgentManager1 = QStringLiteral("org.bluez.AgentManager1");
    orgBluezObex = QStringLiteral("org.bluez.obex");
    orgBluezObexClient1 = QStringLiteral("org.bluez.obex.Client1");
    orgBluezObexAgentManager1 = QStringLiteral("org.bluez.obex.AgentManager1");
    orgBluezObexSession1 = QStringLiteral("org.bluez.obex.Session1");
    orgBluezObexTransfer1 = QStringLiteral("org.bluez.obex.Transfer1");
}

Q_GLOBAL_STATIC(GlobalData, globalData)

// For fakebluez tests
QBLUEZ_EXPORT void qbluez_initFakeBluezTestRun()
{
    globalData->testRun = true;
    globalData->orgBluez = QStringLiteral("org.qbluez.fakebluez");
}

QString Strings::orgFreedesktopDBus()
{
    return globalData->orgFreedesktopDBus;
}

QString Strings::orgBluez()
{
    return globalData->orgBluez;
}

QString Strings::orgBluezAdapter1()
{
    return globalData->orgBluezAdapter1;
}

QString Strings::orgBluezDevice1()
{
    return globalData->orgBluezDevice1;
}

QString Strings::orgBluezAgentManager1()
{
    return globalData->orgBluezAgentManager1;
}

QString Strings::orgBluezObex()
{
    return globalData->orgBluezObex;
}

QString Strings::orgBluezObexClient1()
{
    return globalData->orgBluezObexClient1;
}

QString Strings::orgBluezObexAgentManager1()
{
    return globalData->orgBluezObexAgentManager1;
}

QString Strings::orgBluezObexSession1()
{
    return globalData->orgBluezObexSession1;
}

QString Strings::orgBluezObexTransfer1()
{
    return globalData->orgBluezObexTransfer1;
}

QDBusConnection DBusConnection::orgBluez()
{
    if (globalData->testRun) {
        return QDBusConnection::sessionBus();
    }
    return QDBusConnection::systemBus();
}

QDBusConnection DBusConnection::orgBluezObex()
{
    return QDBusConnection::sessionBus();
}

QStringList stringListToUpper(const QStringList &list)
{
    QStringList converted;
    Q_FOREACH (const QString &str, list) {
        converted.append(str.toUpper());
    }
    return converted;
}

} // namespace QBluez
