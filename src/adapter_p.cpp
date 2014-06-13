#include "adapter_p.h"
#include "adapter.h"

using namespace QBluez;

AdapterPrivate::AdapterPrivate(const QString &path, Adapter *parent)
    : QObject(parent)
    , q(parent)
    , m_loaded(false)
    , m_path(path)
    , m_adapterClass(0)
    , m_powered(0)
    , m_discoverable(false)
    , m_discoverableTimeout(0)
    , m_pairable(false)
    , m_pairableTimeout(0)
{
    m_bluezAdapter = new BluezAdapter(QStringLiteral("org.bluez"), m_path,
                                      QDBusConnection::systemBus(), this);
    m_dbusProperties = new DBusProperties(QStringLiteral("org.bluez"), m_path,
                                          QDBusConnection::systemBus(), this);
}

void AdapterPrivate::addDevice(Device *device)
{
    m_devices.append(device);
    emit q->deviceFound(device);
}

void AdapterPrivate::removeDevice(Device *device)
{
    m_devices.removeOne(device);
    emit q->deviceRemoved(device);
}

void AdapterPrivate::setDBusProperty(const QString &name, const QVariant &value)
{
    m_dbusProperties->Set(QStringLiteral("org.bluez.Adapter1"), name, QDBusVariant(value));
}
