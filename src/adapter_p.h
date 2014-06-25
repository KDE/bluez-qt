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
    Q_OBJECT

public:
    AdapterPrivate(const QString &path, Adapter *parent);

    void addDevice(Device *device);
    void removeDevice(Device *device);

    void load();
    QDBusPendingReply<> setDBusProperty(const QString &name, const QVariant &value);
    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

    Adapter *q;
    BluezAdapter *m_bluezAdapter;
    DBusProperties *m_dbusProperties;
    bool m_loaded;

    QString m_path;
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
    QList<Device *> m_devices;
    QString m_modalias;

Q_SIGNALS:
    void loaded(AdapterPrivate *adapter);
    void loadError(const QString &errorString);
};

} // namespace QBluez

#endif // QBLUEZ_ADAPTER_P_H
