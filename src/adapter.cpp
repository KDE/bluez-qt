#include "adapter.h"
#include "adapter_p.h"

using namespace QBluez;

Adapter::Adapter(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new AdapterPrivate(path, this))
{
}

Adapter::~Adapter()
{
    delete d;
}

bool Adapter::isLoaded() const
{
    return d->m_loaded;
}

LoadAdapterJob *Adapter::load() const
{
    return new LoadAdapterJob(d);
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

SetPropertyJob *Adapter::setAlias(const QString &alias)
{
    return new SetPropertyJob(QStringLiteral("Alias"), alias, d);
}

quint32 Adapter::adapterClass() const
{
    return d->m_adapterClass;
}

SetPropertyJob *Adapter::setAdapterClass(quint32 adapterClass)
{
    return new SetPropertyJob(QStringLiteral("Class"), adapterClass, d);
}

bool Adapter::isPowered() const
{
    return d->m_powered;
}

SetPropertyJob *Adapter::setPowered(bool powered)
{
    return new SetPropertyJob(QStringLiteral("Powered"), powered, d);
}

bool Adapter::isDiscoverable() const
{
    return d->m_discoverable;
}

SetPropertyJob *Adapter::setDiscoverable(bool discoverable)
{
    return new SetPropertyJob(QStringLiteral("Discoverable"), discoverable, d);
}

quint32 Adapter::discoverableTimeout() const
{
    return d->m_discoverableTimeout;
}

SetPropertyJob *Adapter::setDiscoverableTimeout(quint32 timeout)
{
    return new SetPropertyJob(QStringLiteral("DiscoverableTimeout"), timeout, d);
}

bool Adapter::isPairable() const
{
    return d->m_pairable;
}

SetPropertyJob *Adapter::setPairable(bool pairable)
{
    return new SetPropertyJob(QStringLiteral("Pairable"), pairable, d);
}

quint32 Adapter::pairableTimeout() const
{
    return d->m_pairableTimeout;
}

SetPropertyJob *Adapter::setPairableTimeout(quint32 timeout)
{
    return new SetPropertyJob(QStringLiteral("PairableTimeout"), timeout, d);
}

bool Adapter::isDiscovering()
{
    return d->m_discovering;
}

QStringList Adapter::UUIDs() const
{
    return d->m_UUIDs;
}

QString Adapter::modalias() const
{
    return d->m_modalias;
}

QList<Device *> Adapter::devices() const
{
    return d->m_devices;
}

void Adapter::removeDevice(Device *device)
{
    // FIXME !
    d->removeDevice(device);
}

void Adapter::startDiscovery()
{
    d->m_bluezAdapter->StartDiscovery();
}

void Adapter::stopDiscovery()
{
    d->m_bluezAdapter->StopDiscovery();
}
