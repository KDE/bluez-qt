#ifndef QBLUEZ_DEVICE_H
#define QBLUEZ_DEVICE_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class Adapter;
class LoadDeviceJob;
class DevicePrivate;

class QBLUEZ_EXPORT Device : public QObject
{
    Q_OBJECT

public:
    virtual ~Device();

    bool isLoaded() const;
    LoadDeviceJob *load() const;

    QString path() const;

private:
    Device(const QString &path, QObject *parent = 0);

    DevicePrivate *const d;

    friend class DevicePrivate;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICE_H
