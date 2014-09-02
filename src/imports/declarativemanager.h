#ifndef DECLARATIVEMANAGER_H
#define DECLARATIVEMANAGER_H

#include "manager.h"

#include <QQmlListProperty>

class DeclarativeManager : public QBluez::Manager
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QBluez::Adapter> adapters READ declarativeAdapters)
    Q_PROPERTY(QQmlListProperty<QBluez::Device> devices READ declarativeDevices)

public:
    DeclarativeManager(QObject *parent = 0);

    QQmlListProperty<QBluez::Adapter> declarativeAdapters();
    QQmlListProperty<QBluez::Device> declarativeDevices();

Q_SIGNALS:
    void initialized();
    void initializeError(const QString &errorText);

private:
    void initJobResult(QBluez::InitManagerJob *job);
};

#endif // DECLARATIVEMANAGER_H

