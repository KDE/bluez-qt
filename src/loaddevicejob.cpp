#include "loaddevicejob.h"
#include "device_p.h"

using namespace QBluez;

LoadDeviceJob::LoadDeviceJob(DevicePrivate *dd, QObject *parent)
    : Job(parent)
    , dd(dd)
{
}

Device *LoadDeviceJob::device() const
{
    return dd->q;
}

void LoadDeviceJob::doStart()
{
    if (dd->m_loaded) {
        emitResult();
        return;
    }

    dd->initProperties();

    const QDBusPendingReply<QVariantMap> &call = dd->m_dbusProperties->GetAll(QStringLiteral("org.bluez.Device1"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ = ]() {
        const QDBusPendingReply<QVariantMap> &reply = *watcher;
        if (reply.isError()) {
            setError(UserDefinedError);
            setErrorText(reply.error().message());
            emitResult();
            return;
        }

        const QVariantMap &properties = reply.value();

        dd->m_address = properties.value(QStringLiteral("Address")).toString();
        dd->m_name = properties.value(QStringLiteral("Name")).toString();
        dd->m_alias = properties.value(QStringLiteral("Alias")).toString();
        dd->m_deviceClass = properties.value(QStringLiteral("Class")).toUInt();
        dd->m_appearance = properties.value(QStringLiteral("Appearance")).toUInt();
        dd->m_icon = properties.value(QStringLiteral("Icon")).toString();
        dd->m_paired = properties.value(QStringLiteral("Paired")).toBool();
        dd->m_trusted = properties.value(QStringLiteral("Trusted")).toBool();
        dd->m_blocked = properties.value(QStringLiteral("Blocked")).toBool();
        dd->m_legacyPairing = properties.value(QStringLiteral("LegacyPairing")).toBool();
        dd->m_rssi = properties.value(QStringLiteral("RSSI")).toInt();
        dd->m_connected = properties.value(QStringLiteral("Connected")).toBool();
        dd->m_uuids = properties.value(QStringLiteral("UUIDs")).toStringList();
        dd->m_modalias = properties.value(QStringLiteral("Modalias")).toString();

        dd->m_loaded = true;

        emitResult();
    });
}
