/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2018 Manuel Weichselbaumer <mincequi@web.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLUEZQT_MEDIAENDPOINT_H
#define BLUEZQT_MEDIAENDPOINT_H

#include <QObject>

#include "bluezqt_export.h"
#include "request.h"

class QDBusObjectPath;

namespace BluezQt
{

/**
 * @class BluezQt::MediaEndpoint MediaEndpoint.h <BluezQt/MediaEndpoint>
 *
 * Bluetooth MediaEndpoint.
 *
 * This class represents a Bluetooth MediaEndpoint.
 */
class BLUEZQT_EXPORT MediaEndpoint : public QObject
{
    Q_OBJECT

public:
    /** Role which this MediaEndpoint acts as. */
    enum class Role {
        AudioSource,
        AudioSink
    };

    /** Codec which this MediaEndpoint supports. */
    enum class Codec {
        Sbc,
        Aac
    };

    /** Configuration for MediaEndpoint construction. */
    struct Configuration {
        Role role;
        Codec codec;
    };

    /**
     * Creates a new MediaEndpoint object.
     *
     * @param parent
     */
    explicit MediaEndpoint(const Configuration &configuration, QObject *parent = nullptr);

    /**
     * Destroys a MediaEndpoint object.
     */
    ~MediaEndpoint();

    /**
     * D-Bus object path of the MediaEndpoint.
     *
     * The path where the MediaEndpoint will be registered.
     *
     * @note You must provide valid object path!
     *
     * @return object path of MediaEndpoint
     */
    virtual QDBusObjectPath objectPath() const;

    /**
     * Properties of the endpoint.
     *
     * @return Properties of the endpoint
     */
    virtual const QVariantMap &properties() const;

    /**
     * Set configuration for the transport.
     *
     * @param transport transport to be configured
     * @param properties properties to be set for transport
     */
    virtual void setConfiguration(const QString &transportObjectPath, const QVariantMap &properties);

    /**
     * Select preferable configuration from the supported capabilities.
     *
     * @note There is no need to cache the selected configuration since on success
     *       the configuration is send back as parameter of SetConfiguration.
     *
     * @param capabilities supported capabilities
     * @param request request to be used for sending reply
     */
    virtual void selectConfiguration(const QByteArray &capabilities, const Request<QByteArray> &request);

    /**
     * Clear transport configuration.
     */
    virtual void clearConfiguration(const QString &transportObjectPath);

    /**
     * Indicates that the MediaEndpoint was unregistered.
     *
     * This method gets called when the Bluetooth daemon
     * unregisters the MediaEndpoint.
     *
     * An MediaEndpoint can use it to do cleanup tasks. There is no need
     * to unregister the MediaEndpoint, because when this method gets called
     * it has already been unregistered.
     */
    virtual void release();

Q_SIGNALS:
    /**
     * Indicates that configuration was selected.
     */
    void configurationSelected(const QByteArray &capabilities, const QByteArray &configuration);

    /**
     * Indicates that configuration was set for transport.
     */
    void configurationSet(const QString &transportObjectPath, const QVariantMap &properties);

    /**
     * Indicates that configuration was cleared for transport.
     */
    void configurationCleared(const QString &transportObjectPath);

private:
    class MediaEndpointPrivate *const d;
};

} // namespace BluezQt

#endif // BLUEZQT_MEDIAENDPOINT_H
