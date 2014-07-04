#include "device.h"
#include "device_p.h"

namespace QBluez
{

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
    return d->m_loaded;
}

LoadDeviceJob *Device::load()
{
    return new LoadDeviceJob(d, this);
}

QString Device::path() const
{
    return d->m_path;
}

QString Device::address() const
{
    return d->m_address;
}

QString Device::name() const
{
    return d->m_name;
}

QString Device::friendlyName() const
{
    if (alias().isEmpty() || alias() == name()) {
        return name();
    }
    if (name().isEmpty()) {
        return alias();
    }
    return QString(QStringLiteral("%1 (%2)")).arg(alias(), name());
}

QString Device::alias() const
{
    return d->m_alias;
}

SetPropertyJob *Device::setAlias(const QString &alias)
{
    return new SetPropertyJob(QStringLiteral("Alias"), alias, d);
}

quint32 Device::deviceClass() const
{
    return d->m_deviceClass;
}

quint16 Device::appearance() const
{
    return d->m_appearance;
}

QString Device::icon() const
{
    return d->m_icon.isEmpty() ? QStringLiteral("preferences-system-bluetooth") : d->m_icon;
}

bool Device::isPaired() const
{
    return d->m_paired;
}

bool Device::isTrusted() const
{
    return d->m_trusted;
}

SetPropertyJob *Device::setTrusted(bool trusted)
{
    return new SetPropertyJob(QStringLiteral("Trusted"), trusted, d);
}

bool Device::isBlocked() const
{
    return d->m_blocked;
}

SetPropertyJob *Device::setBlocked(bool blocked)
{
    return new SetPropertyJob(QStringLiteral("Blocked"), blocked, d);
}

bool Device::hasLegacyPairing() const
{
    return d->m_legacyPairing;
}

qint16 Device::rssi() const
{
    return d->m_rssi;
}

bool Device::isConnected() const
{
    return d->m_connected;
}

QStringList Device::uuids() const
{
    return d->m_uuids;
}

QString Device::modalias() const
{
    return d->m_modalias;
}

Adapter *Device::adapter() const
{
    return d->m_adapter;
}

void Device::pair()
{
    d->m_bluezDevice->Pair();
}

void Device::connect()
{
    d->m_bluezDevice->Connect();
}

void Device::disconnect()
{
    d->m_bluezDevice->Disconnect();
}

} // namespace QBluez
