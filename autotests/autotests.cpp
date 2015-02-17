#include "autotests.h"

#include <QDir>
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>
#include <QDBusPendingCall>
#include <QDBusServiceWatcher>

QProcess *FakeBluez::s_process = 0;

class StartJob : public QObject
{
    Q_OBJECT

public:
    explicit StartJob();

    void exec();

private Q_SLOTS:
    void processError(QProcess::ProcessError error);
    void processFinished(int code, QProcess::ExitStatus status);

private:
    QString m_fakebluezPath;
    QEventLoop m_eventLoop;
};

StartJob::StartJob()
    : QObject(0)
    , m_fakebluezPath(qApp->applicationDirPath() + QStringLiteral("/fakebluez/fakebluez"))
{
}

void StartJob::exec()
{
    QDBusServiceWatcher watcher(QStringLiteral("org.qbluez.fakebluez"),
                                QDBusConnection::sessionBus(),
                                QDBusServiceWatcher::WatchForRegistration);

    QObject::connect(&watcher, &QDBusServiceWatcher::serviceRegistered, &m_eventLoop, &QEventLoop::quit);
    QObject::connect(FakeBluez::s_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    QObject::connect(FakeBluez::s_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));

    FakeBluez::s_process->start(m_fakebluezPath);

    m_eventLoop.exec();
}

void StartJob::processError(QProcess::ProcessError error)
{
    QString errorString;
    switch (error) {
    case QProcess::FailedToStart:
        errorString = QStringLiteral("Failed to Start");
        break;
    case QProcess::Crashed:
        errorString = QStringLiteral("Crashed");
        break;
    case QProcess::Timedout:
        errorString = QStringLiteral("Timedout");
        break;
    default:
        errorString = QStringLiteral("Unknown");
    }

    qWarning() << "Fakebluez binary:" << m_fakebluezPath;
    qWarning() << "Process error:" << error << errorString;
    qWarning() << "Error output:" << FakeBluez::s_process->readAllStandardError();

    m_eventLoop.quit();
}

void StartJob::processFinished(int code, QProcess::ExitStatus status)
{
    QString statusString = status == QProcess::NormalExit ? QStringLiteral("Normal Exit") : QStringLiteral("Crash Exit");

    qWarning() << "Fakebluez binary:" << m_fakebluezPath;
    qWarning() << "Process finished:" << code << statusString;
    qWarning() << "Error output:" << FakeBluez::s_process->readAllStandardError();

    m_eventLoop.quit();
}

void FakeBluez::start()
{
    if (isRunning()) {
        return;
    }
    if (!s_process) {
        s_process = new QProcess();
    }

    StartJob job;
    job.exec();
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
    if (!isRunning()) {
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.qbluez.fakebluez"),
                        QStringLiteral("/"),
                        QStringLiteral("org.qbluez.fakebluez.Test"),
                        QStringLiteral("runTest"));

    call << testName;
    QDBusConnection::sessionBus().call(call);
}

void FakeBluez::runAction(const QString &object, const QString &actionName, const QVariantMap &properties)
{
    if (!isRunning()) {
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.qbluez.fakebluez"),
                        QStringLiteral("/"),
                        QStringLiteral("org.qbluez.fakebluez.Test"),
                        QStringLiteral("runAction"));

    call << object;
    call << actionName;
    call << properties;

    QEventLoop eventLoop;
    QDBusConnection::sessionBus().connect(QStringLiteral("org.qbluez.fakebluez"),
                                          QStringLiteral("/"),
                                          QStringLiteral("org.qbluez.fakebluez.Test"),
                                          QStringLiteral("actionFinished"),
                                          &eventLoop, SLOT(quit()));

    QDBusConnection::sessionBus().asyncCall(call);
    eventLoop.exec();
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

#include "autotests.moc"
