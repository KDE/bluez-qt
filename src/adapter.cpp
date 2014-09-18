#include "adapter.h"
#include "adapter_p.h"
#include "device.h"
#include "device_p.h"
#include "pendingcall.h"

namespace QBluez
{

Adapter::Adapter(const QString &path, const QVariantMap &properties, QObject *parent)
    : QObject(parent)
    , d(new AdapterPrivate(path, properties, this))
{
    qRegisterMetaType<QBluez::Adapter*>("QBluez::Adapter*");
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
    return new PendingCall(d->setDBusProperty(QStringLiteral("Alias"), alias),
                           PendingCall::ReturnVoid, this);
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
    return new PendingCall(d->setDBusProperty(QStringLiteral("Powered"), powered),
                           PendingCall::ReturnVoid, this);
}

bool Adapter::isDiscoverable() const
{
    return d->m_discoverable;
}

PendingCall *Adapter::setDiscoverable(bool discoverable)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Discoverable"), discoverable),
                           PendingCall::ReturnVoid, this);
}

quint32 Adapter::discoverableTimeout() const
{
    return d->m_discoverableTimeout;
}

PendingCall *Adapter::setDiscoverableTimeout(quint32 timeout)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("DiscoverableTimeout"), timeout),
                           PendingCall::ReturnVoid, this);
}

bool Adapter::isPairable() const
{
    return d->m_pairable;
}

PendingCall *Adapter::setPairable(bool pairable)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Pairable"), pairable),
                           PendingCall::ReturnVoid, this);
}

quint32 Adapter::pairableTimeout() const
{
    return d->m_pairableTimeout;
}

PendingCall *Adapter::setPairableTimeout(quint32 timeout)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("PairableTimeout"), timeout),
                           PendingCall::ReturnVoid, this);
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

QList<Device*> Adapter::devices() const
{
    return d->m_devices;
}

Device *Adapter::deviceForAddress(const QString &address) const
{
    Q_FOREACH (Device *device, d->m_devices) {
        if (device->address() == address) {
            return device;
        }
    }
    return Q_NULLPTR;
}

PendingCall *Adapter::startDiscovery()
{
    return new PendingCall(d->m_bluezAdapter->StartDiscovery(),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Adapter::stopDiscovery()
{
    return new PendingCall(d->m_bluezAdapter->StopDiscovery(),
                           PendingCall::ReturnVoid, this);
}

PendingCall *Adapter::removeDevice(Device *device)
{
    return new PendingCall(d->m_bluezAdapter->RemoveDevice(QDBusObjectPath(device->ubi())),
                           PendingCall::ReturnVoid, this);
}

} // namespace QBluez
