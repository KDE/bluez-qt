#ifndef QBLUEZ_ADAPTERINFO_H
#define QBLUEZ_ADAPTERINFO_H

#include <QObject>
#include <QList>

#include "qbluez_export.h"

namespace QBluez
{

class Adapter;
class DeviceInfo;

class QBLUEZ_EXPORT AdapterInfo : public QObject
{
    Q_OBJECT

public:
    virtual ~AdapterInfo();

    //FetchAdapterJob *fetchAdapter() const;

    QString path() const;

    QList<DeviceInfo *> devices() const;

Q_SIGNALS:
    void deviceAdded(DeviceInfo *device);
    void deviceRemoved(DeviceInfo *device);

private:
    AdapterInfo(const QString &path, QObject *parent = 0);

    void addDevice(DeviceInfo *device);
    void removeDevice(DeviceInfo *device);

    QString m_path;
    Adapter *m_adapter;
    QList<DeviceInfo *> m_devices;

    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTERINFO_H
