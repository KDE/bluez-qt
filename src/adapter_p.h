#ifndef QBLUEZ_ADAPTER_P_H
#define QBLUEZ_ADAPTER_P_H

#include <QObject>
#include <QStringList>

#include "bluezadapter1.h"
#include "dbusproperties.h"

namespace QBluez
{

typedef org::bluez::Adapter1 BluezAdapter;
typedef org::freedesktop::DBus::Properties DBusProperties;

class Device;
class Adapter;

class AdapterPrivate : public QObject
{
public:
    explicit AdapterPrivate(const QString &path, const QVariantMap &properties, Adapter *parent);

    void init(const QVariantMap &properties);

    void addDevice(Device *device);
    void removeDevice(Device *device);

    QDBusPendingReply<> setDBusProperty(const QString &name, const QVariant &value);
    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);
    void uuidsPropertyChanged(const QStringList &value);

    Adapter *q;
    BluezAdapter *m_bluezAdapter;
    DBusProperties *m_dbusProperties;

    QString m_address;
    QString m_name;
    QString m_alias;
    quint32 m_adapterClass;
    bool m_powered;
    bool m_discoverable;
    quint32 m_discoverableTimeout;
    bool m_pairable;
    quint32 m_pairableTimeout;
    bool m_discovering;
    QStringList m_uuids;
    QList<Device*> m_devices;
    QString m_modalias;
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_P_H
