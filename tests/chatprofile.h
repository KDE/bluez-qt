#include "profile.h"
#include "device.h"

class QLocalSocket;

class ChatProfile : public QBluez::Profile
{
    Q_OBJECT

public:
    ChatProfile(QObject *parent);

    QDBusObjectPath objectPath() const;
    QString uuid() const;

    void newConnection(QBluez::Device *device, const QDBusUnixFileDescriptor &fd, const QVariantMap &properties, const QBluez::Request<> &request);
    void requestDisconnection(QBluez::Device *device, const QBluez::Request<> &request);
    void release();

private Q_SLOTS:
    void socketReadyRead();
    void socketDisconnected();
    void writeToSocket();

private:
    QLocalSocket *m_socket;
};
