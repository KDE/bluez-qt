/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_OBEXSESSION_H
#define BLUEZQT_OBEXSESSION_H

#include <QObject>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

class QDBusObjectPath;

namespace BluezQt
{
class PendingCall;

/*!
 * \inmodule BluezQt
 * \class BluezQt::ObexSession
 * \inheaderfile BluezQt/ObexSession
 * \brief OBEX session.
 *
 * This class represents an OBEX session.
 */
class BLUEZQT_EXPORT ObexSession : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::ObexSession::source */
    Q_PROPERTY(QString source READ source)
    /*! \property BluezQt::ObexSession::destination */
    Q_PROPERTY(QString destination READ destination)
    /*! \property BluezQt::ObexSession::channel */
    Q_PROPERTY(quint8 channel READ channel)
    /*! \property BluezQt::ObexSession::target */
    Q_PROPERTY(QString target READ target)
    /*! \property BluezQt::ObexSession::root */
    Q_PROPERTY(QString root READ root)

public:
    ~ObexSession() override;

    /*!
     * Returns a shared pointer from this.
     */
    ObexSessionPtr toSharedPtr() const;

    /*!
     * Returns the D-Bus object path of the session.
     */
    QDBusObjectPath objectPath() const;

    /*!
     * Returns the address of the Bluetooth adapter.
     * \sa Manager::adapterForAddress()
     */
    QString source() const;

    /*!
     * Returns the address of the Bluetooth device.
     * \sa Manager::deviceForAddress()
     */
    QString destination() const;

    /*!
     * Returns the Bluetooth channel.
     */
    quint8 channel() const;

    /*!
     * Returns the target UUID.
     */
    QString target() const;

    /*!
     * Returns the root path.
     */
    QString root() const;

    /*!
     * Returns the remote device capabilities.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::NotSupported
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns QString pending call.
     */
    PendingCall *getCapabilities();

private:
    BLUEZQT_NO_EXPORT explicit ObexSession(const QString &path, const QVariantMap &properties);

    std::unique_ptr<class ObexSessionPrivate> const d;

    friend class ObexSessionPrivate;
    friend class ObexManagerPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXSESSION_H
