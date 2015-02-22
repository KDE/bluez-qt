#include "profile.h"
#include "device.h"

class QLocalSocket;

class ChatProfile : public BluezQt::Profile
{
    Q_OBJECT

public:
    ChatProfile(QObject *parent);

    QDBusObjectPath objectPath() const;
    QString uuid() const;

    void newConnection(BluezQt::DevicePtr device, const QDBusUnixFileDescriptor &fd, const QVariantMap &properties, const BluezQt::Request<> &request);
    void requestDisconnection(BluezQt::DevicePtr device, const BluezQt::Request<> &request);
    void release();

private Q_SLOTS:
    void socketReadyRead();
    void socketDisconnected();
    void writeToSocket();

private:
    QSharedPointer<QLocalSocket> m_socket;
};
