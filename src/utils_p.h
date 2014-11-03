#ifndef QBLUEZ_UTILS_P_H
#define QBLUEZ_UTILS_P_H

#include "device.h"

class QString;
class QStringList;
class QDBusConnection;

namespace QBluez
{

namespace Strings
{

QString orgFreedesktopDBus();
QString orgBluez();
QString orgBluezAdapter1();
QString orgBluezDevice1();
QString orgBluezAgentManager1();
QString orgBluezObex();
QString orgBluezObexClient1();
QString orgBluezObexAgentManager1();
QString orgBluezObexSession1();
QString orgBluezObexTransfer1();

}

namespace DBusConnection
{

QDBusConnection orgBluez();
QDBusConnection orgBluezObex();

}

QStringList stringListToUpper(const QStringList &list);
Device::DeviceType classToType(quint32 classNum);

} // namespace QBluez

#endif // QBLUEZ_UTILS_P_H
