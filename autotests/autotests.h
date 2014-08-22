#ifndef AUTOTESTS_H
#define AUTOTESTS_H

#include <QtTest/QTest>
#include <QSignalSpy>
#include <QProcess>
#include <QDBusMessage>
#include <QDBusConnection>

class FakeBluez
{
public:
    static void start();
    static void stop();

    static bool isRunning();
    static void runTest(const QString &testName);
    static void runAction(const QString &object, const QString &actionName, const QVariantMap &properties = QVariantMap());

    static QProcess *s_process;
};

void verifyPropertiesChangedSignal(const QSignalSpy &spy, const QString &propertyName, const QVariant &propertyValue);

#endif // AUTOTESTS_H
