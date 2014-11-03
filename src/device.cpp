#include "device.h"
#include "device_p.h"
#include "pendingcall.h"
#include "utils_p.h"

namespace QBluez
{

Device::Device(const QString &path, const QVariantMap &properties, Adapter *adapter, QObject *parent)
    : QObject(parent)
    , d(new DevicePrivate(path, properties, adapter, this))
{
    qRegisterMetaType<QBluez::Device*>("QBluez::Device*");
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

Device::DeviceType Device::deviceType() const
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

PendingCall *Device::connectDevice()
{
    return new PendingCall(d->m_bluezDevice->Connect(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::disconnectDevice()
{
    return new PendingCall(d->m_bluezDevice->Disconnect(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::connectProfile(const QString &uuid)
{
    return new PendingCall(d->m_bluezDevice->ConnectProfile(uuid), PendingCall::ReturnVoid, this);
}

PendingCall *Device::disconnectProfile(const QString &uuid)
{
    return new PendingCall(d->m_bluezDevice->DisconnectProfile(uuid), PendingCall::ReturnVoid, this);
}

PendingCall *Device::pair()
{
    return new PendingCall(d->m_bluezDevice->Pair(), PendingCall::ReturnVoid, this);
}

PendingCall *Device::cancelPairing()
{
    return new PendingCall(d->m_bluezDevice->CancelPairing(), PendingCall::ReturnVoid, this);
}

QString typeToString(Device::DeviceType type)
{
    switch (type) {
    case Device::Any:
        return QStringLiteral("any");
    case Device::Phone:
        return QStringLiteral("phone");
    case Device::Modem:
        return QStringLiteral("modem");
    case Device::Computer:
        return QStringLiteral("computer");
    case Device::Network:
        return QStringLiteral("network");
    case Device::Headset:
        return QStringLiteral("headset");
    case Device::Headphones:
        return QStringLiteral("headphones");
    case Device::OtherAudio:
        return QStringLiteral("audio");
    case Device::Keyboard:
        return QStringLiteral("keyboard");
    case Device::Mouse:
        return QStringLiteral("mouse");
    case Device::Camera:
        return QStringLiteral("camera");
    case Device::Printer:
        return QStringLiteral("printer");
    case Device::Joypad:
        return QStringLiteral("joypad");
    case Device::Tablet:
        return QStringLiteral("tablet");
    default:
        return QStringLiteral("any");
    }
}

Device::DeviceType stringToType(const QString &stringType)
{
    if (stringType == QLatin1String("any")) {
        return Device::Any;
    } else if (stringType == QLatin1String("phone")) {
        return Device::Phone;
    } else if (stringType == QLatin1String("modem")) {
        return Device::Modem;
    } else if (stringType == QLatin1String("computer")) {
        return Device::Computer;
    } else if (stringType == QLatin1String("network")) {
        return Device::Network;
    } else if (stringType == QLatin1String("headset")) {
        return Device::Headset;
    } else if (stringType == QLatin1String("headphones")) {
        return Device::Headphones;
    } else if (stringType == QLatin1String("audio")) {
        return Device::OtherAudio;
    } else if (stringType == QLatin1String("keyboard")) {
        return Device::Keyboard;
    } else if (stringType == QLatin1String("mouse")) {
        return Device::Mouse;
    } else if (stringType == QLatin1String("camera")) {
        return Device::Camera;
    } else if (stringType == QLatin1String("printer")) {
        return Device::Printer;
    } else if (stringType == QLatin1String("joypad")) {
        return Device::Joypad;
    } else if (stringType == QLatin1String("tablet")) {
        return Device::Tablet;
    }
    return Device::Any;
}

} // namespace QBluez
