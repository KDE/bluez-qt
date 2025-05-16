/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * SPDX-FileCopyrightText: 2018 Manuel Weichselbaumer <mincequi@web.de>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_MEDIAENDPOINT_H
#define BLUEZQT_MEDIAENDPOINT_H

#include <QObject>

#include "bluezqt_export.h"
#include "request.h"

#include <memory>

class QDBusObjectPath;

namespace BluezQt
{
/*!
 * \inmodule BluezQt
 * \class BluezQt::MediaEndpoint
 * \inheaderfile BluezQt/MediaEndpoint
 * \brief Bluetooth MediaEndpoint.
 *
 * This class represents a Bluetooth MediaEndpoint.
 */
class BLUEZQT_EXPORT MediaEndpoint : public QObject
{
    Q_OBJECT

public:
    /*!
     * \enum BluezQt::MediaEndpoint::Role
     * \brief Role which this MediaEndpoint acts as.
     * \value AudioSource
     * \value AudioSink
     */
    enum class Role {
        AudioSource,
        AudioSink,
    };

    // KF6 TODO: use types from mediatypes.h
    /*!
     * \enum BluezQt::MediaEndpoint::Codec
     * \brief Codec which this MediaEndpoint supports.
     * \value Sbc
     * \value Aac
     */
    enum class Codec {
        Sbc,
        Aac,
    };

    /*!
     * Configuration for MediaEndpoint construction.
     * \inmodule BluezQt
     */
    struct Configuration {
        /*!
         *
         */
        Role role;

        /*!
         *
         */
        Codec codec;
    };

    /*!
     * Creates a new MediaEndpoint object with the given \a configuration
     * as a child of \a parent.
     */
    explicit MediaEndpoint(const Configuration &configuration, QObject *parent = nullptr);

    ~MediaEndpoint() override;

    /*!
     * Returns the D-Bus object path of the MediaEndpoint.
     *
     * The path where the MediaEndpoint will be registered.
     *
     * \note You must provide valid object path!
     */
    virtual QDBusObjectPath objectPath() const;

    /*!
     * Returns the properties of the endpoint.
     */
    virtual const QVariantMap &properties() const;

    /*!
     * Set configuration for the transport at the \a transportObjectPath
     * with the given \a properties.
     */
    virtual void setConfiguration(const QString &transportObjectPath, const QVariantMap &properties);

    /*!
     * Select preferable configuration from the supported \a capabilities with a \a request to be used for sending the reply.
     *
     * \note There is no need to cache the selected configuration since on success
     *       the configuration is send back as parameter of SetConfiguration.
     */
    virtual void selectConfiguration(const QByteArray &capabilities, const Request<QByteArray> &request);

    /*!
     * Clear the transport configuration at \a transportObjectPath.
     */
    virtual void clearConfiguration(const QString &transportObjectPath);

    /*!
     * Indicates that the MediaEndpoint was unregistered.
     *
     * This method gets called when the Bluetooth daemon
     * unregisters the MediaEndpoint.
     *
     * A MediaEndpoint can use it to do cleanup tasks. There is no need
     * to unregister the MediaEndpoint, because when this method gets called
     * it has already been unregistered.
     */
    virtual void release();

Q_SIGNALS:
    /*!
     * Indicates that the configuration was selected.
     */
    void configurationSelected(const QByteArray &capabilities, const QByteArray &configuration);

    /*!
     * Indicates that the configuration was set for transport.
     */
    void configurationSet(const QString &transportObjectPath, const QVariantMap &properties);

    /*!
     * Indicates that the configuration was cleared for transport.
     */
    void configurationCleared(const QString &transportObjectPath);

private:
    std::unique_ptr<class MediaEndpointPrivate> const d;
};

} // namespace BluezQt

#endif // BLUEZQT_MEDIAENDPOINT_H
