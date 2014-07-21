#ifndef QBLUEZ_OBEXSESSION_H
#define QBLUEZ_OBEXSESSION_H

#include <QObject>

#include "qbluez_export.h"

namespace QBluez
{

class PendingCall;

class QBLUEZ_EXPORT ObexSession : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source)
    Q_PROPERTY(QString destination READ destination)
    Q_PROPERTY(quint8 channel READ channel)
    Q_PROPERTY(QString target READ target)
    Q_PROPERTY(QString root READ root)

public:
    ~ObexSession();

    QString source() const;

    QString destination() const;

    quint8 channel() const;

    QString target() const;

    QString root() const;

    // Possible errors: NotSupported, Failed
    // Return: QString
    PendingCall *getCapabilities();

private:
    explicit ObexSession(const QString &path, QObject *parent = 0);

    class ObexSessionPrivate *const d;

    friend class ObexSessionPrivate;
    friend class ObexTransferPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXSESSION_H
