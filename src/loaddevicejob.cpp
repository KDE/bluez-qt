#include "loaddevicejob.h"
#include "device_p.h"
#include "debug_p.h"
#include "utils_p.h"

namespace QBluez
{

class LoadDeviceJobPrivate : public QObject
{
    Q_OBJECT

public:
    LoadDeviceJobPrivate(LoadDeviceJob *q, DevicePrivate *device);

    void doStart();

    LoadDeviceJob *q;
    DevicePrivate *m_device;
};

LoadDeviceJobPrivate::LoadDeviceJobPrivate(LoadDeviceJob *q, DevicePrivate *device)
    : QObject(q)
    , q(q)
    , m_device(device)
{
}

void LoadDeviceJobPrivate::doStart()
{
    if (m_device->m_loaded) {
        q->emitResult();
        return;
    }

    m_device->initProperties();

    const QDBusPendingReply<QVariantMap> &call = m_device->m_dbusProperties->GetAll(QStringLiteral("org.bluez.Device1"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ this, watcher ]() {
        const QDBusPendingReply<QVariantMap> &reply = *watcher;
        watcher->deleteLater();

        if (reply.isError()) {
            qCWarning(QBLUEZ) << "LoadDeviceJob Error:" << reply.error().message();

            q->setError(LoadDeviceJob::UserDefinedError);
            q->setErrorText(reply.error().message());
            q->emitResult();
            return;
        }

        const QVariantMap &properties = reply.value();

        m_device->m_address = properties.value(QStringLiteral("Address")).toString();
        m_device->m_name = properties.value(QStringLiteral("Name")).toString();
        m_device->m_alias = properties.value(QStringLiteral("Alias")).toString();
        m_device->m_deviceClass = properties.value(QStringLiteral("Class")).toUInt();
        m_device->m_appearance = properties.value(QStringLiteral("Appearance")).toUInt();
        m_device->m_icon = properties.value(QStringLiteral("Icon")).toString();
        m_device->m_paired = properties.value(QStringLiteral("Paired")).toBool();
        m_device->m_trusted = properties.value(QStringLiteral("Trusted")).toBool();
        m_device->m_blocked = properties.value(QStringLiteral("Blocked")).toBool();
        m_device->m_legacyPairing = properties.value(QStringLiteral("LegacyPairing")).toBool();
        m_device->m_rssi = properties.value(QStringLiteral("RSSI")).toInt();
        m_device->m_connected = properties.value(QStringLiteral("Connected")).toBool();
        m_device->m_uuids = stringListToUpper(properties.value(QStringLiteral("UUIDs")).toStringList());
        m_device->m_modalias = properties.value(QStringLiteral("Modalias")).toString();

        m_device->m_loaded = true;

        q->emitResult();
    });
}

LoadDeviceJob::LoadDeviceJob(DevicePrivate *device, QObject *parent)
    : Job(parent)
    , d(new LoadDeviceJobPrivate(this, device))
{
}

LoadDeviceJob::~LoadDeviceJob()
{
    delete d;
}

Device *LoadDeviceJob::device() const
{
    return d->m_device->q;
}

void LoadDeviceJob::doStart()
{
    d->doStart();
}

void LoadDeviceJob::doEmitResult()
{
    Q_EMIT result(this);
}

} // namespace QBluez

#include "loaddevicejob.moc"
