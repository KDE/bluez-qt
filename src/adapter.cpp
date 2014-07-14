#include "adapter.h"
#include "adapter_p.h"
#include "device.h"
#include "device_p.h"
#include "pendingcall.h"

namespace QBluez
{

Adapter::Adapter(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new AdapterPrivate(path, this))
{
}

Adapter::~Adapter()
{
    delete d;
}

QString Adapter::ubi() const
{
    return d->m_bluezAdapter->path();
}

QString Adapter::address() const
{
    return d->m_address;
}

QString Adapter::name() const
{
    return d->m_name;
}

QString Adapter::alias() const
{
    return d->m_alias;
}

PendingCall *Adapter::setAlias(const QString &alias)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Alias"), alias), this);
}

quint32 Adapter::adapterClass() const
{
    return d->m_adapterClass;
}

bool Adapter::isPowered() const
{
    return d->m_powered;
}

PendingCall *Adapter::setPowered(bool powered)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Powered"), powered), this);
}

bool Adapter::isDiscoverable() const
{
    return d->m_discoverable;
}

PendingCall *Adapter::setDiscoverable(bool discoverable)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Discoverable"), discoverable), this);
}

quint32 Adapter::discoverableTimeout() const
{
    return d->m_discoverableTimeout;
}

PendingCall *Adapter::setDiscoverableTimeout(quint32 timeout)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("DiscoverableTimeout"), timeout), this);
}

bool Adapter::isPairable() const
{
    return d->m_pairable;
}

PendingCall *Adapter::setPairable(bool pairable)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Pairable"), pairable), this);
}

quint32 Adapter::pairableTimeout() const
{
    return d->m_pairableTimeout;
}

PendingCall *Adapter::setPairableTimeout(quint32 timeout)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("PairableTimeout"), timeout), this);
}

bool Adapter::isDiscovering()
{
    return d->m_discovering;
}

QStringList Adapter::uuids() const
{
    return d->m_uuids;
}

QString Adapter::modalias() const
{
    return d->m_modalias;
}

QList<Device *> Adapter::devices() const
{
    return d->m_devices;
}

PendingCall *Adapter::startDiscovery()
{
    return new PendingCall(d->m_bluezAdapter->StartDiscovery(), this);
}

PendingCall *Adapter::stopDiscovery()
{
    return new PendingCall(d->m_bluezAdapter->StopDiscovery(), this);
}

PendingCall *Adapter::removeDevice(Device *device)
{
    return new PendingCall(d->m_bluezAdapter->RemoveDevice(QDBusObjectPath(device->ubi())), this);
}

} // namespace QBluez
