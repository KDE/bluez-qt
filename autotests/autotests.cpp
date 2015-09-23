/*
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "autotests.h"
#include "device.h"
#include "adapter.h"
#include "mediaplayer.h"
#include "mediaplayertrack.h"
#include "bluezqt_dbustypes.h"

#include <QDir>
#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

#include <QTest>
#include <QDBusReply>
#include <QDBusMetaType>
#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusServiceWatcher>
#include <QDBusConnectionInterface>

QProcess *FakeBluez::s_process = Q_NULLPTR;

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
    , m_fakebluezPath(QFINDTESTDATA("fakebluez/fakebluez"))
{
}

void StartJob::exec()
{
    QDBusServiceWatcher watcher(QStringLiteral("org.kde.bluezqt.test"),
                                QDBusConnection::sessionBus(),
                                QDBusServiceWatcher::WatchForRegistration);

    connect(&watcher, &QDBusServiceWatcher::serviceRegistered, &m_eventLoop, &QEventLoop::quit);
    connect(FakeBluez::s_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    connect(FakeBluez::s_process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));

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

    QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.kde.bluezqt.test"),
                        QStringLiteral("/"),
                        QStringLiteral("org.kde.bluezqt.fakebluez.Test"),
                        QStringLiteral("runTest"));

    call << testName;
    QDBusConnection::sessionBus().call(call);
}

void FakeBluez::runAction(const QString &object, const QString &actionName, const QVariantMap &properties)
{
    if (!isRunning()) {
        return;
    }

    QDBusMessage call = QDBusMessage::createMethodCall(QStringLiteral("org.kde.bluezqt.test"),
                        QStringLiteral("/"),
                        QStringLiteral("org.kde.bluezqt.fakebluez.Test"),
                        QStringLiteral("runAction"));

    call << object;
    call << actionName;
    call << properties;

    QDBusConnection::sessionBus().call(call);
}

void Autotests::registerMetatypes()
{
    qRegisterMetaType<BluezQt::DevicePtr>("DevicePtr");
    qRegisterMetaType<BluezQt::AdapterPtr>("AdapterPtr");
    qRegisterMetaType<BluezQt::MediaPlayerPtr>("MediaPlayerPtr");
}

void Autotests::verifyPropertiesChangedSignal(const QSignalSpy &spy, const QString &propertyName, const QVariant &propertyValue)
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
