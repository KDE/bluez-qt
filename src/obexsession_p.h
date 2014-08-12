#include "obexsession1.h"
#include "dbusproperties.h"

namespace QBluez
{

typedef org::bluez::obex::Session1 BluezSession;
typedef org::freedesktop::DBus::Properties DBusProperties;

class ObexSessionPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ObexSessionPrivate(ObexSession *q, const QString &path);

    void init();
    void getPropertiesFinished(QDBusPendingCallWatcher *watcher);

    ObexSession *q;
    BluezSession *m_bluezSession;

    QString m_source;
    QString m_destination;
    quint8 m_channel;
    QString m_target;
    QString m_root;

Q_SIGNALS:
    void initError(const QString &errorText);
    void initFinished();
};

} // namespace QBluez
