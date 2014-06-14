#include "loadadapterjob.h"
#include "adapter_p.h"

using namespace QBluez;

LoadAdapterJob::LoadAdapterJob(AdapterPrivate *dd, QObject *parent)
    : Job(parent)
    , dd(dd)
{
}

Adapter *LoadAdapterJob::adapter() const
{
    return dd->q;
}

void LoadAdapterJob::doStart()
{
    if (dd->m_loaded) {
        emitResult();
        return;
    }

    dd->initProperties();

    const QDBusPendingReply<QVariantMap> &call = dd->m_dbusProperties->GetAll(QStringLiteral("org.bluez.Adapter1"));
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ = ]() {
        const QDBusPendingReply<QVariantMap> &reply = *watcher;
        if (reply.isError()) {
            setErrorText(reply.error().message());
            emitResult();
            return;
        }

        const QVariantMap &properties = reply.value();

        dd->m_address = properties.value(QStringLiteral("Address")).toString();
        dd->m_name = properties.value(QStringLiteral("Name")).toString();
        dd->m_alias = properties.value(QStringLiteral("Alias")).toString();
        dd->m_adapterClass = properties.value(QStringLiteral("Class")).toUInt();
        dd->m_powered = properties.value(QStringLiteral("Powered")).toBool();
        dd->m_discoverable = properties.value(QStringLiteral("Discoverable")).toBool();
        dd->m_discoverableTimeout = properties.value(QStringLiteral("DiscoverableTimeout")).toUInt();
        dd->m_pairable = properties.value(QStringLiteral("Pairable")).toBool();
        dd->m_pairableTimeout = properties.value(QStringLiteral("PairableTimeout")).toUInt();
        dd->m_discovering = properties.value(QStringLiteral("Discovering")).toBool();
        dd->m_UUIDs = properties.value(QStringLiteral("UUIDs")).toStringList();
        dd->m_modalias = properties.value(QStringLiteral("Modalias")).toString();

        dd->m_loaded = true;

        emitResult();
    });
}
