#include "chatprofile.h"
#include "manager.h"
#include "initmanagerjob.h"
#include "pendingcall.h"

#include <QDebug>
#include <QTimer>
#include <QLocalSocket>
#include <QCoreApplication>
#include <QDBusObjectPath>
#include <QDBusConnection>
#include <QDBusUnixFileDescriptor>

// This is a server for BluetoothChat application from Android samples.
//
// On successful connection, it sends "test-data" message to the client
// and then writes all messages from client to the standard output.
//
// https://developer.android.com/samples/BluetoothChat/index.html

ChatProfile::ChatProfile(QObject *parent)
    : BluezQt::Profile(parent)
    , m_socket(0)
{
    setName(QStringLiteral("BluetoothChatSecure"));
    setChannel(0);
}

QDBusObjectPath ChatProfile::objectPath() const
{
    return QDBusObjectPath(QStringLiteral("/ChatProfile"));
}

QString ChatProfile::uuid() const
{
    return QStringLiteral("fa87c0d0-afac-11de-8a39-0800200c9a66");
}

void ChatProfile::newConnection(BluezQt::Device *device, const QDBusUnixFileDescriptor &fd, const QVariantMap &properties, const BluezQt::Request<> &request)
{
    qDebug() << "Connect" << device->name() << properties;

    m_socket = createSocket(fd);
    if (!m_socket->isValid()) {
        delete m_socket;
        request.cancel();
        return;
    }

    connect(m_socket, &QLocalSocket::readyRead, this, &ChatProfile::socketReadyRead);
    connect(m_socket, &QLocalSocket::disconnected, this, &ChatProfile::socketDisconnected);
    QTimer::singleShot(1000, this, SLOT(writeToSocket()));

    request.accept();
}

void ChatProfile::requestDisconnection(BluezQt::Device *device, const BluezQt::Request<> &request)
{
    qDebug() << "Disconnect" << device->name();

    request.accept();
}

void ChatProfile::release()
{
    qDebug() << "Release";
}

void ChatProfile::socketReadyRead()
{
    qDebug() << "Read:" << m_socket->socketDescriptor() << m_socket->readAll();
}

void ChatProfile::socketDisconnected()
{
    qDebug() << "Socket disconnected";
    m_socket->deleteLater();
    m_socket = 0;
}

void ChatProfile::writeToSocket()
{
    qDebug() << "Writing" << m_socket->socketDescriptor();
    m_socket->write(QByteArrayLiteral("test-data"));
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    BluezQt::Manager *manager = new BluezQt::Manager();
    BluezQt::InitManagerJob *initJob = manager->init();
    initJob->exec();

    if (initJob->error()) {
        qWarning() << "Error initializing manager:" << initJob->errorText();
        return 1;
    }

    BluezQt::PendingCall *call = manager->registerProfile(new ChatProfile(&app));
    call->waitForFinished();

    if (call->error()) {
        qWarning() << "Error registering profile" << call->errorText();
        return 1;
    }

    qDebug() << "Profile registered";

    return app.exec();
}
