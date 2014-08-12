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
    explicit LoadDeviceJobPrivate(LoadDeviceJob *q, DevicePrivate *device);

    void doStart();
    void loaded();
    void loadError(const QString &errorText);

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

    m_device->load();
    connect(m_device, &DevicePrivate::loaded, this, &LoadDeviceJobPrivate::loaded);
    connect(m_device, &DevicePrivate::loadError, this, &LoadDeviceJobPrivate::loadError);
}

void LoadDeviceJobPrivate::loaded()
{
    q->emitResult();
}

void LoadDeviceJobPrivate::loadError(const QString &errorText)
{
    qCWarning(QBLUEZ) << "LoadDeviceJob Error:" << errorText;

    q->setError(LoadDeviceJob::UserDefinedError);
    q->setErrorText(errorText);
    q->emitResult();
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
