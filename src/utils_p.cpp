#include "utils_p.h"

#include <QStringList>

namespace QBluez
{

QString s_orgFreedesktopDBus;
QString s_orgBluez;
QString s_orgBluezAdapter1;
QString s_orgBluezDevice1;
QString s_orgBluezAgentManager1;
QString s_orgBluezObex;
QString s_orgBluezObexClient1;
QString s_orgBluezObexAgentManager1;
QString s_orgBluezObexSession1;
QString s_orgBluezObexTransfer1;

void initStrings()
{
    if (!s_orgFreedesktopDBus.isEmpty()) {
        return;
    }

    s_orgFreedesktopDBus = QStringLiteral("org.freedesktop.DBus");
    s_orgBluez = QStringLiteral("org.bluez");
    s_orgBluezAdapter1 = QStringLiteral("org.bluez.Adapter1");
    s_orgBluezDevice1 = QStringLiteral("org.bluez.Device1");
    s_orgBluezAgentManager1 = QStringLiteral("org.bluez.AgentManager1");
    s_orgBluezObex = QStringLiteral("org.bluez.obex");
    s_orgBluezObexClient1 = QStringLiteral("org.bluez.obex.Client1");
    s_orgBluezObexAgentManager1 = QStringLiteral("org.bluez.obex.AgentManager1");
    s_orgBluezObexSession1 = QStringLiteral("org.bluez.obex.Session1");
    s_orgBluezObexTransfer1 = QStringLiteral("org.bluez.obex.Transfer1");
}

QString Strings::orgFreedesktopDBus()
{
    initStrings();
    return s_orgFreedesktopDBus;
}

QString Strings::orgBluez()
{
    initStrings();
    return s_orgBluez;
}

QString Strings::orgBluezAdapter1()
{
    initStrings();
    return s_orgBluezAdapter1;
}

QString Strings::orgBluezDevice1()
{
    initStrings();
    return s_orgBluezDevice1;
}

QString Strings::orgBluezAgentManager1()
{
    initStrings();
    return s_orgBluezAgentManager1;
}

QString Strings::orgBluezObex()
{
    initStrings();
    return s_orgBluezObex;
}

QString Strings::orgBluezObexClient1()
{
    initStrings();
    return s_orgBluezObexClient1;
}

QString Strings::orgBluezObexAgentManager1()
{
    initStrings();
    return s_orgBluezObexAgentManager1;
}

QString Strings::orgBluezObexSession1()
{
    initStrings();
    return s_orgBluezObexSession1;
}

QString Strings::orgBluezObexTransfer1()
{
    initStrings();
    return s_orgBluezObexTransfer1;
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
