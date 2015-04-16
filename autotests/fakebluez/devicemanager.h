#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include <QObject>

class ObjectManager;

class DeviceManager : public QObject
{
    Q_OBJECT

public:
    explicit DeviceManager(ObjectManager *parent = Q_NULLPTR);

    void runAction(const QString &actionName, const QVariantMap &properties);

private:
    void runCreateAdapterAction(const QVariantMap &properties);
    void runCreateDeviceAction(const QVariantMap &properties);
    void runRemoveAdapterAction(const QVariantMap &properties);
    void runRemoveDeviceAction(const QVariantMap &properties);
    void runChangeAdapterProperty(const QVariantMap &properties);
    void runChangeDeviceProperty(const QVariantMap &properties);

    ObjectManager *m_objectManager;

};

#endif // DEVICEMANAGER_H
