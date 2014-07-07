#ifndef ADAPTERTEST_H
#define ADAPTERTEST_H

#include <QObject>
#include <QList>

#include "bluezadapter1.h"
#include "dbusproperties.h"

#include "manager.h"
#include "adapter.h"

class QSignalSpy;

class AdapterTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void getPropertiesTest();
    void setAliasTest();
    void setPoweredTest();
    void setDiscoverableTest();
    void setDiscoverableTimeoutTest();
    void setPairableTest();
    void setPairableTimeoutTest();

    void discoveryTest();

private:
    void verifyPropertiesChangedSignal(const QSignalSpy &spy, const QString &propertyName, const QVariant &propertyValue);

    struct AdapterUnit
    {
        QBluez::Adapter *adapter;
        org::bluez::Adapter1 *dbusAdapter;
        org::freedesktop::DBus::Properties *dbusProperties;
    };

    QList<AdapterUnit> m_units;
};

#endif // ADAPTERTEST_H
