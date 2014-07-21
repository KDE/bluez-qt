#include "obextransfer.h"

#include "obextransfer1.h"
#include "dbusproperties.h"

namespace QBluez
{

typedef org::bluez::obex::Transfer1 BluezTransfer;
typedef org::freedesktop::DBus::Properties DBusProperties;

class ObexTransferPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ObexTransferPrivate(ObexTransfer *q, const QString &path);

    void init();

    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

    ObexTransfer *q;
    BluezTransfer *m_bluezTransfer;
    DBusProperties *m_dbusProperties;

    ObexTransfer::Status m_status;
    ObexSession* m_session;
    QString m_name;
    QString m_type;
    quint64 m_time;
    quint64 m_size;
    quint64 m_transferred;
    QString m_fileName;

Q_SIGNALS:
    void initError(const QString &errorText);
    void initFinished();
};

} // namespace QBluez
