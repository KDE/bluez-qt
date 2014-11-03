#include "qbluezextensionplugin.h"
#include "declarativemanager.h"
#include "declarativedevicesmodel.h"
#include "adapter.h"
#include "device.h"
#include "pendingcall.h"

#include <QtQml>

QBluezExtensionPlugin::QBluezExtensionPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

static QJSValue services_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)

    QJSValue object = scriptEngine->newObject();
    object.setProperty(QStringLiteral("SerialPort"), QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("DialupNetworking"), QStringLiteral("00001103-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("ObexObjectPush"), QStringLiteral("00001105-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("ObexFileTransfer"), QStringLiteral("00001106-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("AudioSource"), QStringLiteral("0000110A-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("AudioVideoRemoteControlTarget"), QStringLiteral("0000110C-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("AdvancedAudioDistributionProtocol"), QStringLiteral("0000110D-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("AudioVideoRemoteControl"), QStringLiteral("0000110E-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("HeadsetAudioGateway"), QStringLiteral("00001112-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("Nap"), QStringLiteral("00001116-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("HandsfreeAudioGateway"), QStringLiteral("0000111F-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("SimAccess"), QStringLiteral("0000112D-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("PhonebookAccessPse"), QStringLiteral("0000112F-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("MessageAccessServer"), QStringLiteral("00001132-0000-1000-8000-00805F9B34FB"));
    object.setProperty(QStringLiteral("PnpInformation"), QStringLiteral("00001200-0000-1000-8000-00805F9B34FB"));
    return object;
}

void QBluezExtensionPlugin::registerTypes(const char *uri)
{
    using namespace QBluez;

    Q_ASSERT(QLatin1String(uri) == QLatin1String("org.qbluez"));

    qmlRegisterType<DeclarativeManager>(uri, 1, 0, "Manager");
    qmlRegisterType<DeclarativeDevicesModel>(uri, 1, 0, "DevicesModel");
    qmlRegisterUncreatableType<Adapter>(uri, 1, 0, "Adapter", QStringLiteral("Adapter cannot be created"));
    qmlRegisterUncreatableType<Device>(uri, 1, 0, "Device", QStringLiteral("Device cannot be created"));
    qmlRegisterUncreatableType<PendingCall>(uri, 1, 0, "PendingCall", QStringLiteral("PendingCall cannot be created"));
    qmlRegisterSingletonType(uri, 1, 0, "Services", services_singletontype_provider);
}
