/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_PROFILE_H
#define BLUEZQT_PROFILE_H

#include <QObject>

#include "bluezqt_export.h"
#include "request.h"
#include "types.h"

#include <memory>

class QLocalSocket;
class QDBusObjectPath;
class QDBusUnixFileDescriptor;

namespace BluezQt
{
class Device;

/*!
 * \inmodule BluezQt
 * \class BluezQt::Profile
 * \inheaderfile BluezQt/Profile
 * \brief Bluetooth profile.
 *
 * This class represents a Bluetooth profile.
 *
 * It is only needed to reimplement pure virtual functions.
 * You don't need to set any additional properties.
 *
 * But you may need to specify at least channel number or PSM in case it couldn't be
 * determined from UUID. It is also a good idea to provide name for the profile.
 *
 * Setting the channel number with setChannel() will make the profile use RFCOMM, while
 * setting the PSM with setPsm() will make the profile use L2CAP.
 *
 * \note The return value of requests will be sent asynchronously with Request class.
 *       It is also possible to cancel/reject all requests.
 *
 */
class BLUEZQT_EXPORT Profile : public QObject
{
    Q_OBJECT

    /*! \property BluezQt::Profile::uuid */
    Q_PROPERTY(QString uuid READ uuid)

public:
    /*!
     * \enum BluezQt::Profile::LocalRole
     * \brief Local role to identify sides in asymmetric profiles.
     * \value ClientRole
     *        Indicates that this is a client.
     * \value ServerRole
     *        Indicates that this is a server.
     */
    enum LocalRole {
        ClientRole,
        ServerRole,
    };

    /*!
     * Creates a new Profile object as a child of \a parent.
     */
    explicit Profile(QObject *parent = nullptr);

    ~Profile() override;

    /*!
     * Returns the D-Bus object path of the profile.
     *
     * The path where the profile will be registered.
     *
     * \note You must provide valid object path!
     */
    virtual QDBusObjectPath objectPath() const = 0;

    /*!
     * Returns the UUID of the profile.
     */
    virtual QString uuid() const = 0;

    /*!
     * Sets the human readable \a name of the profile.
     */
    void setName(const QString &name);

    /*!
     * Sets the primary \a service class UUID (if different from profile UUID).
     */
    void setService(const QString &service);

    /*!
     * Sets the local \a role to identify the side.
     *
     * For asymmetric profiles that do not have UUIDs available
     * to uniquely identify each side this parameter allows
     * specifying the precise local role.
     */
    void setLocalRole(LocalRole role);

    /*!
     * Sets the RFCOMM \a channel number.
     *
     * Available channel number range is 0-31.
     *
     * Setting channel number to 0 will automatically choose
     * the correct channel number for profile UUID.
     */
    void setChannel(quint16 channel);

    /*!
     * Sets the L2CAP \a psm port number.
     *
     * PSM (Protocol Service Multiplexer) is a port number
     * in L2CAP.
     *
     * Setting PSM to 0 will automatically choose the correct
     * PSM for the profile UUID.
     */
    void setPsm(quint16 psm);

    /*!
     * Sets whether the pairing is required to connect with parameter \a require.
     */
    void setRequireAuthentication(bool require);

    /*!
     * Sets whether the authorization is required to connect with parameter \a require.
     */
    void setRequireAuthorization(bool require);

    /*!
     * Sets whether the profile will \a autoConnect.
     *
     * In case of a client UUID this will force connection
     * of the RFCOMM or L2CAP channels when a remote device
     * is connected.
     */
    void setAutoConnect(bool autoConnect);

    /*!
     * Sets an SDP \a serviceRecord.
     *
     * This allows to provide a manual SDP record, otherwise it will
     * be generated automatically.
     */
    void setServiceRecord(const QString &serviceRecord);

    /*!
     * Sets the profile \a version.
     */
    void setVersion(quint16 version);

    /*!
     * Sets the profile \a features.
     */
    void setFeatures(quint16 features);

    /*!
     * Creates a socket from the given file descriptor \a fd.
     * \sa newConnection()
     */
    QSharedPointer<QLocalSocket> createSocket(const QDBusUnixFileDescriptor &fd);

    /*!
     * Requests the new connection.
     *
     * Common properties:
     * \list
     * \li quint16 Version - Profile version
     * \li quint16 Features - Profile features
     * \endlist
     *
     * To create socket from fd, you can use:
     * \code
     *  QSharedPointer<QLocalSocket> socket = createSocket(fd);
     *  if (!socket->isValid()) {
     *      delete socket;
     *      request.cancel();
     *      return;
     *  }
     * \endcode
     *
     * \a device The device that requested the connection.
     *
     * \a fd The socket file descriptor.
     *
     * \a properties Additional properties.
     *
     * \a request The request to be used for sending a reply.
     */
    virtual void newConnection(DevicePtr device, const QDBusUnixFileDescriptor &fd, const QVariantMap &properties, const Request<> &request);

    /*!
     * Requests the disconnection of the profile.
     *
     * This method gets called when a profile gets disconnected.
     *
     * \a device The device to be disconnected.
     *
     * \a request The request to be used for sending a reply.
     */
    virtual void requestDisconnection(DevicePtr device, const Request<> &request);

    /*!
     * Indicates that the profile was unregistered.
     *
     * This method gets called when the Bluetooth daemon
     * unregisters the profile.
     *
     * A profile can use it to do cleanup tasks. There is no need
     * to unregister the profile, because when this method gets called
     * it has already been unregistered.
     */
    virtual void release();

private:
    std::unique_ptr<class ProfilePrivate> const d;

    friend class Manager;
};

} // namespace BluezQt

#endif // BLUEZQT_PROFILE_H
