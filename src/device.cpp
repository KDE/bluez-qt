#include "device.h"
#include "device_p.h"

using namespace QBluez;

Device::Device(const QString &path, Adapter *adapter, QObject *parent)
    : QObject(parent)
    , d(new DevicePrivate(path, adapter, this))
{
}

Device::~Device()
{
    delete d;
}

bool Device::isLoaded() const
{
    return false;
}

LoadDeviceJob *Device::load() const
{
    return 0;
}

QString Device::address() const
{
    return d->m_address;
}

Adapter *Device::adapter() const
{
    return d->m_adapter;
}
