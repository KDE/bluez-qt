#include "utils_p.h"

#include <QStringList>

namespace QBluez
{

QStringList stringListToUpper(const QStringList &list)
{
    QStringList converted;
    Q_FOREACH (const QString &str, list) {
        converted.append(str.toUpper());
    }
    return converted;
}

} // namespace QBluez
