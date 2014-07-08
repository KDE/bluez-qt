#ifndef ADAPTERTEST_H
#define ADAPTERTEST_H

#include <QObject>
#include <QList>

#include "bluezadapter1_tst.h"
#include "dbusproperties_tst.h"

#include "manager.h"
#include "adapter.h"

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
    struct AdapterUnit
    {
        QBluez::Adapter *adapter;
        org::bluez::Adapter1 *dbusAdapter;
        org::freedesktop::DBus::Properties *dbusProperties;
    };

    QList<AdapterUnit> m_units;
};

#endif // ADAPTERTEST_H
