#include "autotests.h"

#include <QDir>
#include <QtQml>
#include <QtQuickTest>

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

    void runAction(const QString &object, const QString &actionName, const QVariantMap &properties = QVariantMap())
    {
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

    QDir sourceDir(QStringLiteral(__FILE__));
    sourceDir.cdUp();
    const QString &testsDir = sourceDir.absoluteFilePath(QStringLiteral("qml"));

    sourceDir.cdUp();
    sourceDir.cd(QStringLiteral("build/src/imports"));
    qputenv("QML2_IMPORT_PATH", sourceDir.absolutePath().toUtf8());

    BluezQt::bluezqt_initFakeBluezTestRun();

    return quick_test_main(argc, argv, "qmltests", testsDir.toUtf8().constData());
}

#include "qmltests.moc"
