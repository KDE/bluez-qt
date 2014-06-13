#include "device.h"
#include "device_p.h"

using namespace QBluez;

Device::Device(const QString &path, QObject *parent)
    : QObject(parent)
    , d(new DevicePrivate(this))
{
    d->m_path = path;
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

QString Device::path() const
{
    return d->m_path;
}
