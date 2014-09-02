#include "qbluezextensionplugin.h"
#include "declarativemanager.h"
#include "declarativedevicesmodel.h"
#include "adapter.h"
#include "device.h"

#include <QtQml>

QBluezExtensionPlugin::QBluezExtensionPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

void QBluezExtensionPlugin::registerTypes(const char *uri)
{
    using namespace QBluez;

    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.qbluez"));

    qmlRegisterType<DeclarativeManager>(uri, 1, 0, "Manager");
    qmlRegisterType<DeclarativeDevicesModel>(uri, 1, 0, "DevicesModel");
    qmlRegisterUncreatableType<Adapter>(uri, 1, 0, "Adapter", QStringLiteral("Adapter cannot be created"));
    qmlRegisterUncreatableType<Device>(uri, 1, 0, "Device", QStringLiteral("Device cannot be created"));
}
