#ifndef QBLUEZ_DEVICE_P_H
#define QBLUEZ_DEVICE_P_H

#include <QObject>

namespace QBluez
{

class Device;

class DevicePrivate : public QObject
{
    Q_OBJECT

public:
    DevicePrivate(Device *parent);

    Device *q;
    QString m_path;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICE_P_H
