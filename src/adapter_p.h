#ifndef QBLUEZ_ADAPTER_P_H
#define QBLUEZ_ADAPTER_P_H

#include <QObject>

namespace QBluez
{

class Device;
class Adapter;

class AdapterPrivate : public QObject
{
    Q_OBJECT

public:
    AdapterPrivate(Adapter *parent);

    Adapter *q;
    QString m_path;
    QList<Device *> m_devices;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_P_H
