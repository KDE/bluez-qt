#include "qbluezextensionplugin.h"
#include "declarativemanager.h"
#include "declarativedevicesmodel.h"
#include "adapter.h"
#include "device.h"
#include "pendingcall.h"
#include "services.h"

#include <QtQml>

QBluezExtensionPlugin::QBluezExtensionPlugin(QObject *parent)
    : QQmlExtensionPlugin(parent)
{
}

static QJSValue services_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)

    QJSValue object = scriptEngine->newObject();
    object.setProperty(QStringLiteral("SerialPort"), QBluez::Services::SerialPort);
    object.setProperty(QStringLiteral("DialupNetworking"), QBluez::Services::DialupNetworking);
    object.setProperty(QStringLiteral("ObexObjectPush"), QBluez::Services::ObexObjectPush);
    object.setProperty(QStringLiteral("ObexFileTransfer"), QBluez::Services::ObexFileTransfer);
    object.setProperty(QStringLiteral("AudioSource"), QBluez::Services::AudioSource);
    object.setProperty(QStringLiteral("AudioVideoRemoteControlTarget"), QBluez::Services::AudioVideoRemoteControlTarget);
    object.setProperty(QStringLiteral("AdvancedAudioDistributionProtocol"), QBluez::Services::AdvancedAudioDistributionProtocol);
    object.setProperty(QStringLiteral("AudioVideoRemoteControl"), QBluez::Services::AudioVideoRemoteControl);
    object.setProperty(QStringLiteral("HeadsetAudioGateway"), QBluez::Services::HeadsetAudioGateway);
    object.setProperty(QStringLiteral("Nap"), QBluez::Services::Nap);
    object.setProperty(QStringLiteral("HandsfreeAudioGateway"), QBluez::Services::HandsfreeAudioGateway);
    object.setProperty(QStringLiteral("SimAccess"), QBluez::Services::SimAccess);
    object.setProperty(QStringLiteral("PhonebookAccessPse"), QBluez::Services::PhonebookAccessPse);
    object.setProperty(QStringLiteral("MessageAccessServer"), QBluez::Services::MessageAccessServer);
    object.setProperty(QStringLiteral("PnpInformation"), QBluez::Services::PnpInformation);
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
