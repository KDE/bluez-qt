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

PendingCall *Device::setAlias(const QString &alias)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Alias"), alias), this);
}

quint32 Device::deviceClass() const
{
    return d->m_deviceClass;
}

DeviceType Device::deviceType() const
{
    return classToType(d->m_deviceClass);
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

PendingCall *Device::setTrusted(bool trusted)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Trusted"), trusted), this);
}

bool Device::isBlocked() const
{
    return d->m_blocked;
}

PendingCall *Device::setBlocked(bool blocked)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Blocked"), blocked), this);
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

PendingCall *Device::connect()
{
    return new PendingCall(d->m_bluezDevice->Connect(), this);
}

PendingCall *Device::disconnect()
{
    return new PendingCall(d->m_bluezDevice->Disconnect(), this);
}

PendingCall *Device::pair()
{
    return new PendingCall(d->m_bluezDevice->Pair(), this);
}

PendingCall *Device::cancelPairing()
{
    return new PendingCall(d->m_bluezDevice->CancelPairing(), this);
}

} // namespace QBluez
