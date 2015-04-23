/*
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#include <QDir>
#include <QtQml>
#include <QtQuickTest>
#include <QDBusObjectPath>

class FakeBluezObject : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void start()
    {
        FakeBluez::start();
    }

    void stop()
    {
        FakeBluez::stop();
    }

    bool isRunning()
    {
        return FakeBluez::isRunning();
    }

    void runTest(const QString &testName)
    {
        FakeBluez::runTest(testName);
    }

    void runAction(const QString &object, const QString &actionName, QVariantMap properties = QVariantMap())
    {
        const QStringList &toDBusObjectPath = properties.value(QStringLiteral("_toDBusObjectPath")).toStringList();
        Q_FOREACH (const QString &name, toDBusObjectPath) {
            const QString &val = properties.value(name).toString();
            properties[name] = QVariant::fromValue(QDBusObjectPath(val));
        }
        properties.remove(QStringLiteral("_toDBusObjectPath"));

        FakeBluez::runAction(object, actionName, properties);
    }
};

class TestController : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    void enableDebugOutput()
    {
        QLoggingCategory::setFilterRules(QStringLiteral("BluezQt=true"));
    }

    void disableDebugOutput()
    {
        QLoggingCategory::setFilterRules(QStringLiteral("BluezQt=false"));
    }

};

static QObject *fakebluez_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new FakeBluezObject;
}

static QObject *testcontroller_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new TestController;
}

namespace BluezQt
{
extern void bluezqt_initFakeBluezTestRun();
}

int main(int argc, char *argv[])
{
    qmlRegisterSingletonType<QObject>("org.kde.bluezqt.fakebluez", 1, 0, "FakeBluez", fakebluez_singleton);
    qmlRegisterSingletonType<QObject>("org.kde.bluezqt.testcontroller", 1, 0, "TestController", testcontroller_singleton);

    BluezQt::bluezqt_initFakeBluezTestRun();

    qputenv("QML2_IMPORT_PATH", QByteArrayLiteral(BLUEZQT_QML_IMPORT_PATH));
    const QString &testsDir = QFileInfo(QFINDTESTDATA("qml/TestUtils.qml")).absolutePath();

    return quick_test_main(argc, argv, "qmltests", testsDir.toUtf8().constData());
}

#include "qmltests.moc"
