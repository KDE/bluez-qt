/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2022 Pontus Sjögren
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef GATTDESCRIPTOR_H
#define GATTDESCRIPTOR_H

#include <QDBusObjectPath>
#include <QObject>

#include "bluezqt_export.h"

#include <memory>

namespace BluezQt
{

class GattCharacteristic;

/*!
 * \inmodule BluezQt
 * \class BluezQt::GattDescriptor
 * \inheaderfile BluezQt/GattDescriptor
 * \brief Bluetooth GATT Descriptor.
 *
 * GATT Descriptors contain additional information and attributes of a GATT characteristic.
 * \since 6.0
 */
class BLUEZQT_EXPORT GattDescriptor : public QObject
{
    Q_OBJECT

public:
    /*!
     * Convenience method to create a User Description
     * \a description for the given \a charactersitic.
     *
     * Returns A pointer to the created descriptor.
     */
    static GattDescriptor *createUserDescription(const QString &description, GattCharacteristic *characteristic);

    /*!
     * Creates a GattDescriptor with the given \a uuid as a child of a \a parent characteristic.
     */
    GattDescriptor(const QString &uuid, GattCharacteristic *parent);

    /*!
     * Creates a GattDescriptor with the given \a uuid and \a flags as a child of a \a parent characteristic.
     */
    GattDescriptor(const QString &uuid, const QStringList &flags, GattCharacteristic *parent);

    /*!
     * Creates a GattDescriptor with the given \a uuid, \a flags
     * and \a initialValue as a child of a \a parent characteristic.
     */
    GattDescriptor(const QString &uuid, const QStringList &flags, const QByteArray &initialValue, GattCharacteristic *parent);

    ~GattDescriptor() override;

    /*!
     * Reads the current value of the descriptor.
     */
    QByteArray readValue();

    /*!
     * Writes the new \a value of the descriptor.
     */
    void writeValue(const QByteArray &value);

    /*!
     * Returns the UUID of the descriptor.
     */
    QString uuid() const;

    /*!
     * Return the D-Bus object path of the parent characteristic.
     */
    QDBusObjectPath characteristic() const;

    /*!
     * Return the flags of the descriptor.
     */
    QStringList flags() const;

protected:
    /*!
     */
    virtual QDBusObjectPath objectPath() const;

private:
    std::unique_ptr<class GattDescriptorPrivate> const d;

    friend class GattManager;
    friend class GattApplicationPrivate;
};

}

#endif
