/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_OBEXMANAGER_H
#define BLUEZQT_OBEXMANAGER_H

#include <QObject>

#include "bluezqt_export.h"
#include "types.h"

#include <memory>

class QDBusObjectPath;

namespace BluezQt
{
class ObexAgent;
class PendingCall;
class InitObexManagerJob;

/*!
 * \inmodule BluezQt
 * \class BluezQt::ObexManager
 * \inheaderfile BluezQt/ObexManager
 * \brief OBEX manager.
 *
 * The entry point to communicate with session BlueZ obex daemon.
 *
 * You must call init() before other functions can be used.
 *
 * \note If manager is not operational, all methods that returns a PendingCall
 *       will fail with PendingCall::InternalError.
 */
class BLUEZQT_EXPORT ObexManager : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::ObexManager::initialized */
    Q_PROPERTY(bool initialized READ isInitialized)
    /*! \property BluezQt::ObexManager::operational */
    Q_PROPERTY(bool operational READ isOperational NOTIFY operationalChanged)
    /*! \property BluezQt::ObexManager::sessions */
    Q_PROPERTY(QList<ObexSessionPtr> sessions READ sessions)

public:
    /*!
     * Creates a new ObexManager object as a child of \a parent.
     */
    explicit ObexManager(QObject *parent = nullptr);

    ~ObexManager() override;

    /*!
     * Creates a new init manager job.
     */
    InitObexManagerJob *init();

    /*!
     * Returns whether the manager is initialized.
     */
    bool isInitialized() const;

    /*!
     * Returns whether the manager is operational.
     *
     * The manager is operational when initialization was successful
     * and the BlueZ session daemon is running.
     */
    bool isOperational() const;

    /*!
     * Returns a list of all sessions.
     */
    QList<ObexSessionPtr> sessions() const;

    /*!
     * Returns a session for the specified \a path.
     *
     * The \a path does not need to be equal to the ObexSession path, startsWith
     * test is performed in the search. That means you can use this method
     * to get ObexSession from path returned by createSession().
     */
    ObexSessionPtr sessionForPath(const QDBusObjectPath &path) const;

    /*!
     * Attempts to start org.bluez.obex service by D-Bus activation.
     *
     * Possible return values are 1 if the service was started,
     * 2 if the service is already running, or error if the service
     * could not be started.
     *
     * Returns quint32 pending call.
     */
    static PendingCall *startService();

public Q_SLOTS:
    /*!
     * Registers an \a agent.
     *
     * This agent will be used to authorize an incoming object push requests.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::AlreadyExists
     * \endlist
     *
     * Returns  void pending call.
     */
    PendingCall *registerAgent(ObexAgent *agent);

    /*!
     * Unregisters an \a agent.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::DoesNotExist
     * \endlist
     *
     * Returns  void pending call
     */
    PendingCall *unregisterAgent(ObexAgent *agent);

    /*!
     * Creates a new OBEX session with the address of the target device
     * \a destination and the given session parameters \a args.
     *
     * The \a args parameter is a dictionary to hold optional or
     * type-specific parameters.
     *
     * Typical parameters:
     * \list
     * \li QString target - type of session to be created
     * \li QString source - device address to be used
     * \endlist
     *
     * Supported targets:
     * \list
     * \li ftp - ObexFileTransfer
     * \li map
     * \li opp - ObexObjectPush
     * \li pbap
     * \li sync
     * \endlist
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::Failed
     * \endlist
     *
     * Returns QDBusObjectPath pending call.
     */
    PendingCall *createSession(const QString &destination, const QVariantMap &args);

    /*!
     * Removes an existing OBEX \a session.
     *
     * Possible errors:
     *
     * \list
     * \li PendingCall::InvalidArguments
     * \li PendingCall::NotAuthorized
     * \endlist
     *
     * Returns void pending call.
     */
    PendingCall *removeSession(const QDBusObjectPath &session);

Q_SIGNALS:
    /*!
     * Indicates that the \a operational state has changed.
     */
    void operationalChanged(bool operational);

    /*!
     * Indicates that the \a session was added.
     */
    void sessionAdded(ObexSessionPtr session);

    /*!
     * Indicates that the \a session was removed.
     */
    void sessionRemoved(ObexSessionPtr session);

private:
    std::unique_ptr<class ObexManagerPrivate> const d;

    friend class ObexManagerPrivate;
    friend class InitObexManagerJobPrivate;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXMANAGER_H
