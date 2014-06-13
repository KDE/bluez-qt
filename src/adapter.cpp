#include "adapter.h"
#include "adapter_p.h"

using namespace QBluez;

Adapter::Adapter(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new AdapterPrivate(this))
{
    d->m_address = path;
}

Adapter::~Adapter()
{
    delete d;
}

bool Adapter::isLoaded() const
{
    return false;
}

LoadAdapterJob *Adapter::load() const
{
    return 0;
}

QString Adapter::address() const
{
    return d->m_address;
}

QList<Device *> Adapter::devices() const
{
    return d->m_devices;
}

void Adapter::addDevice(Device *device)
{
    d->m_devices.append(device);
    emit deviceAdded(device);
}

void Adapter::removeDevice(Device *device)
{
    d->m_devices.removeOne(device);
    emit deviceRemoved(device);
}
