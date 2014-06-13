#ifndef QBLUEZ_DEVICE_P_H
#define QBLUEZ_DEVICE_P_H

#include <QObject>

namespace QBluez
{

class Device;
class Adapter;

class DevicePrivate : public QObject
{
    Q_OBJECT

public:
    DevicePrivate(Device *parent);

    Device *q;
    Adapter *m_adapter;
    QString m_address;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICE_P_H
