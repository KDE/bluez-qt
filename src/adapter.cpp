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

void Adapter::setAlias(const QString &alias)
{
    d->setDBusProperty(QStringLiteral("Alias"), alias);
    d->m_alias = alias;
}

quint32 Adapter::adapterClass() const
{
    return d->m_adapterClass;
}

void Adapter::setAdapterClass(quint32 adapterClass)
{
    d->setDBusProperty(QStringLiteral("Class"), adapterClass);
    d->m_adapterClass = adapterClass;
}

bool Adapter::isPowered() const
{
    return d->m_powered;
}

void Adapter::setPowered(bool powered)
{
    d->setDBusProperty(QStringLiteral("Powered"), powered);
    d->m_powered = powered;
}

bool Adapter::isDiscoverable() const
{
    return d->m_discoverable;
}

void Adapter::setDiscoverable(bool discoverable)
{
    d->setDBusProperty(QStringLiteral("Discoverable"), discoverable);
    d->m_discoverable = discoverable;
}

quint32 Adapter::discoverableTimeout() const
{
    return d->m_discoverableTimeout;
}

void Adapter::setDiscoverableTimeout(quint32 timeout)
{
    d->setDBusProperty(QStringLiteral("DiscoverableTimeout"), timeout);
    d->m_discoverableTimeout = timeout;
}

bool Adapter::isPairable() const
{
    return d->m_pairable;
}

void Adapter::setPairable(bool pairable)
{
    d->setDBusProperty(QStringLiteral("Pairable"), pairable);
    d->m_pairable = pairable;
}

quint32 Adapter::pairableTimeout() const
{
    return d->m_pairableTimeout;
}

void Adapter::setPairableTimeout(quint32 timeout)
{
    d->setDBusProperty(QStringLiteral("PairableTimeout"), timeout);
    d->m_pairableTimeout = timeout;
}

bool Adapter::isDiscovering()
{
    return d->m_discovering;
}

void Adapter::setDiscovering(bool discovering)
{
    d->setDBusProperty(QStringLiteral("Discovering"), discovering);
    d->m_discovering = discovering;
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
