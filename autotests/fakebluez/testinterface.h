#ifndef TESTINTERFACE_H
#define TESTINTERFACE_H

#include <QDBusAbstractAdaptor>

class FakeBluez;

class TestInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.bluezqt.fakebluez.Test")

public:
    explicit TestInterface(FakeBluez *parent);

    void emitActionFinished();

public Q_SLOTS:
    void runTest(const QString &testName);
    void runAction(const QString &object, const QString &actionName, const QVariantMap &properties);

Q_SIGNALS:
    void actionFinished();

private:
    FakeBluez *m_fakeBluez;
};

#endif // TESTINTERFACE_H
