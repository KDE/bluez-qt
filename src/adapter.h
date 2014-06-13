#ifndef QBLUEZ_ADAPTER_H
#define QBLUEZ_ADAPTER_H

#include <QObject>
#include <QList>

#include "qbluez_export.h"

namespace QBluez
{

class Device;
class LoadAdapterJob;
class AdapterPrivate;

class QBLUEZ_EXPORT Adapter : public QObject
{
    Q_OBJECT

public:
    virtual ~Adapter();

    bool isLoaded() const;
    LoadAdapterJob *load() const;

    QString path() const;

    QList<Device *> devices() const;

Q_SIGNALS:
    void deviceAdded(Device *device);
    void deviceRemoved(Device *device);

private:
    Adapter(const QString &path, QObject *parent = 0);

    void addDevice(Device *device);
    void removeDevice(Device *device);

    AdapterPrivate *const d;

    friend class AdapterPrivate;
    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_H
