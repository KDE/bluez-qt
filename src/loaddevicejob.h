#ifndef QBLUEZ_LOADDEVICEJOB_H
#define QBLUEZ_LOADDEVICEJOB_H

#include <QObject>

#include <QBluez/Job>

namespace QBluez
{

class Device;
class DevicePrivate;

class QBLUEZ_EXPORT LoadDeviceJob : public Job
{
    Q_OBJECT

public:
    LoadDeviceJob(DevicePrivate *dd, QObject *parent = 0);

    Device *device() const;

Q_SIGNALS:
    void result(Job *job);

private:
    void doStart() Q_DECL_OVERRIDE;

    DevicePrivate *dd;
};

} // namespace QBluez

#endif // QBLUEZ_LOADDEVICEJOB_H
