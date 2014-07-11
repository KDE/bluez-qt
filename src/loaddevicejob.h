#ifndef QBLUEZ_LOADDEVICEJOB_H
#define QBLUEZ_LOADDEVICEJOB_H

#include <QObject>

#include "job.h"
#include "qbluez_export.h"

namespace QBluez
{

class Device;
class DevicePrivate;

class QBLUEZ_EXPORT LoadDeviceJob : public Job
{
    Q_OBJECT

public:
    ~LoadDeviceJob();

    Device *device() const;

Q_SIGNALS:
    void result(LoadDeviceJob *job);

private:
    explicit LoadDeviceJob(DevicePrivate *device, QObject *parent = 0);

    void doStart() Q_DECL_OVERRIDE;
    void doEmitResult() Q_DECL_OVERRIDE;

    class LoadDeviceJobPrivate *d;
    friend class LoadDeviceJobPrivate;
    friend class Device;
};

} // namespace QBluez

#endif // QBLUEZ_LOADDEVICEJOB_H
