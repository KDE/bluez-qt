#ifndef QBLUEZ_UTILS_P_H
#define QBLUEZ_UTILS_P_H

class QString;
class QStringList;

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

QStringList stringListToUpper(const QStringList &list);

} // namespace QBluez

#endif // QBLUEZ_UTILS_P_H
