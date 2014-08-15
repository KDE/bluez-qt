#include "device.h"
#include "device_p.h"
#include "pendingcall.h"

namespace QBluez
{

Device::Device(const QString &path, const QVariantMap &properties, Adapter *adapter, QObject *parent)
    : QObject(parent)
    , d(new DevicePrivate(path, properties, adapter, this))
{
}

Device::~Device()
{
    delete d;
}

QString Device::ubi() const
{
    return d->m_bluezDevice->path();
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
    return new PendingCall(d->setDBusProperty(QStringLiteral("Alias"), alias),
                           PendingCall::ReturnVoid, this);
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
    return new PendingCall(d->setDBusProperty(QStringLiteral("Trusted"), trusted),
                           PendingCall::ReturnVoid, this);
}

bool Device::isBlocked() const
{
    return d->m_blocked;
}

PendingCall *Device::setBlocked(bool blocked)
{
    return new PendingCall(d->setDBusProperty(QStringLiteral("Blocked"), blocked),
                           PendingCall::ReturnVoid, this);
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
    return new PendingCall(d->m_bluezDevice->Connect(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::disconnect()
{
    return new PendingCall(d->m_bluezDevice->Disconnect(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::pair()
{
    return new PendingCall(d->m_bluezDevice->Pair(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::cancelPairing()
{
    return new PendingCall(d->m_bluezDevice->CancelPairing(), PendingCall::ReturnVoid, this);
}

} // namespace QBluez
