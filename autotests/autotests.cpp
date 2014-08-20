#include "autotests.h"

#include <QDir>
#include <QEventLoop>
#include <QDBusServiceWatcher>

QProcess *FakeBluez::s_process = 0;

void FakeBluez::start()
{
    if (isRunning()) {
        return;
    }
    if (!s_process) {
        s_process = new QProcess();
    }

    QDBusServiceWatcher watcher(QStringLiteral("org.qbluez.fakebluez"),
                                QDBusConnection::sessionBus(),
                                QDBusServiceWatcher::WatchForRegistration);
    QEventLoop eventLoop;
    QObject::connect(&watcher, &QDBusServiceWatcher::serviceRegistered, &eventLoop, &QEventLoop::quit);

    s_process->start(QStringLiteral("fakebluez/fakebluez"));

    eventLoop.exec();
}

void FakeBluez::stop()
{
    if (s_process && s_process->state() == QProcess::Running) {
        s_process->terminate();
        s_process->waitForFinished();
    }
}

bool FakeBluez::isRunning()
{
    return s_process && s_process->state() == QProcess::Running;
}

void FakeBluez::runTest(const QString &testName)
{
    QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.qbluez.fakebluez"),
                        QStringLiteral("/"),
                        QStringLiteral("org.qbluez.fakebluez.Test"),
                        QStringLiteral("runTest"));
    QList<QVariant> args;
    args.append(testName);
    call.setArguments(args);
    QDBusConnection::sessionBus().call(call);
}

void verifyPropertiesChangedSignal(const QSignalSpy &spy, const QString &propertyName, const QVariant &propertyValue)
{
    int changes = 0;

    for (int i = 0; i < spy.count(); ++i) {
        QList<QVariant> arguments = spy.at(i);
        QVariantMap properties = arguments.at(1).toMap();

        QVariantMap::const_iterator it;
        for (it = properties.constBegin(); it != properties.constEnd(); ++it) {
            const QVariant &changedValue = it.value();
            const QString &property = it.key();
            if (property == propertyName && changedValue == propertyValue) {
                changes++;
            }
        }
    }

    QCOMPARE(changes, 1);
}
