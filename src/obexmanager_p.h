#ifndef QBLUEZ_OBEXMANAGER_P_H
#define QBLUEZ_OBEXMANAGER_P_H

#include <QObject>
#include <QTimer>

#include "dbusobjectmanager.h"
#include "obexclient1.h"
#include "obexagentmanager1.h"

namespace QBluez
{

typedef org::bluez::obex::Client1 ObexClient;
typedef org::bluez::obex::AgentManager1 ObexAgentManager;
typedef org::freedesktop::DBus::ObjectManager DBusObjectManager;

class ObexManager;

class ObexManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ObexManagerPrivate(ObexManager *q);
    ~ObexManagerPrivate();

    void init();
    void load();
    void clear();

    void interfacesRemoved(const QDBusObjectPath &objectPath, const QStringList &interfaces);

    ObexManager *q;
    ObexClient *m_obexClient;
    ObexAgentManager *m_obexAgentManager;
    DBusObjectManager *m_dbusObjectManager;
    QTimer *m_timer;

    bool m_initialized;
    bool m_obexRunning;
    bool m_loaded;

Q_SIGNALS:
    void initError(const QString &errorText);
    void initFinished();
};

} // namespace QBluez

#endif // QBLUEZ_OBEXMANAGER_P_H
