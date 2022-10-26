/*
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#define QT_PLUGIN_RESOURCE_INIT_FUNCTION initme

#include "autotests.h"

#include <QDBusObjectPath>

#include <QtQml> // krazy:exclude=includes
#include <QtQuickTest> // krazy:exclude=includes

#ifdef IMPORT_EXTENSIONS_QML_PLUGIN
void initme()
{
    Q_INIT_RESOURCE(bluezqtextensionplugin);
}
#include <QPluginLoader>
#include <qplugin.h>
Q_IMPORT_PLUGIN(BluezQtExtensionPlugin)
#endif

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

    static void processProperties(QVariantMap &properties)
    {
        const QStringList &toDBusObjectPath = properties.value(QStringLiteral("_toDBusObjectPath")).toStringList();
        for (const QString &name : toDBusObjectPath) {
            const QString &val = properties.value(name).toString();
            properties[name] = QVariant::fromValue(QDBusObjectPath(val));
        }
        properties.remove(QStringLiteral("_toDBusObjectPath"));

        QMapIterator<QString, QVariant> it(properties);
        while (it.hasNext()) {
            it.next();
            if (it.key() == QLatin1String("UUIDs")) {
                properties[it.key()] = it.value().toStringList();
            } else if (it.value().type() == QVariant::Map) {
                QVariantMap props = it.value().toMap();
                processProperties(props);
                properties[it.key()] = props;
            }
        }
    }

    void runAction(const QString &object, const QString &actionName, QVariantMap properties = QVariantMap())
    {
        processProperties(properties);
        FakeBluez::runAction(object, actionName, properties);
    }
};

static QObject *fakebluez_singleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new FakeBluezObject;
}

namespace BluezQt
{
extern void bluezqt_initFakeBluezTestRun();
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(bluezqtextensionplugin);
    QFile f(QStringLiteral(":/qt-project.org/imports/org/kde/bluezqt.1.0/qmldir"));
    qWarning() << f.open(QIODevice::ReadOnly);
    qWarning() << f.readAll();
    for (auto a : QDir(QStringLiteral("qrc:/")).entryList())
        qWarning() << a;
    // Q_INIT_RESOURCE(bluezqtextensionplugin);
    for (auto p : QPluginLoader::staticPlugins())
        qWarning() << Q_FUNC_INFO << p.metaData();
    qmlRegisterSingletonType<QObject>("org.kde.bluezqt.fakebluez", 1, 0, "FakeBluez", fakebluez_singleton);

    BluezQt::bluezqt_initFakeBluezTestRun();

    qputenv("QML2_IMPORT_PATH", QByteArrayLiteral(BLUEZQT_QML_IMPORT_PATH));
    const QString &testsDir = QFileInfo(QFINDTESTDATA("qml/tst_device.qml")).absolutePath();

    return quick_test_main(argc, argv, "qmltests", testsDir.toUtf8().constData());
}

#include "qmltests.moc"
