#ifndef QBLUEZ_DEVICEINFO_H
#define QBLUEZ_DEVICEINFO_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class Device;

class QBLUEZ_EXPORT DeviceInfo : public QObject
{
    Q_OBJECT

public:
    virtual ~DeviceInfo();

    //FetchDeviceJob *fetchDevice() const;

    QString path() const;

private:
    DeviceInfo(const QString &path, QObject *parent = 0);

    QString m_path;
    Device *m_device;

    friend class ManagerPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_DEVICEINFO_H
