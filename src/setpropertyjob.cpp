#include "setpropertyjob.h"
#include "adapter_p.h"
#include "device_p.h"

#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

using namespace QBluez;

SetPropertyJob::SetPropertyJob(const QString &name, const QVariant &value, QObject *parent)
    : Job(parent)
    , m_name(name)
    , m_value(value)
{
}

void SetPropertyJob::doStart()
{
    QDBusPendingReply<> call;

    if (AdapterPrivate *adapter = qobject_cast<AdapterPrivate *>(parent())) {
        call = adapter->setDBusProperty(m_name, m_value);
    } else if (DevicePrivate *device = qobject_cast<DevicePrivate *>(parent())) {
        call = device->setDBusProperty(m_name, m_value);
    } else {
        qFatal("SetPropertyJob must be parented to AdapterPrivate or DevicePrivate!");
    }

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, [ this, watcher ]() {
        const QDBusPendingReply<> &reply = *watcher;
        if (reply.isError()) {
            setError(UserDefinedError);
            setErrorText(reply.error().message());
        }
        emitResult();
    });
}
