/*
 * Copyright (C) 2014-2015 David Rosca <nowrep@gmail.com>
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

#ifndef AGENTMANAGERTEST_H
#define AGENTMANAGERTEST_H

#include <QObject>
#include <QDBusObjectPath>

#include "agent.h"
#include "device.h"

class TestAgent : public BluezQt::Agent
{
    Q_OBJECT

public:
    explicit TestAgent(QObject *parent = Q_NULLPTR);

    QDBusObjectPath objectPath() const Q_DECL_OVERRIDE;

    void requestPinCode(BluezQt::DevicePtr device, const BluezQt::Request<QString> &request) Q_DECL_OVERRIDE;
    void displayPinCode(BluezQt::DevicePtr device, const QString &pinCode) Q_DECL_OVERRIDE;
    void requestPasskey(BluezQt::DevicePtr device, const BluezQt::Request<quint32> &request) Q_DECL_OVERRIDE;
    void displayPasskey(BluezQt::DevicePtr device, const QString &passkey, const QString &entered) Q_DECL_OVERRIDE;
    void requestConfirmation(BluezQt::DevicePtr device, const QString &passkey, const BluezQt::Request<> &request) Q_DECL_OVERRIDE;
    void requestAuthorization(BluezQt::DevicePtr device, const BluezQt::Request<> &request) Q_DECL_OVERRIDE;
    void authorizeService(BluezQt::DevicePtr device, const QString &uuid, const BluezQt::Request<> &request) Q_DECL_OVERRIDE;

    void cancel() Q_DECL_OVERRIDE;
    void release() Q_DECL_OVERRIDE;

    BluezQt::DevicePtr m_device;

    // requestPinCode
    bool m_pinRequested;

    // displayPinCode
    QString m_displayedPinCode;

    // requestPasskey
    bool m_passkeyRequested;

    // displayPasskey
    QString m_displayedPasskey;
    QString m_enteredPasskey;

    // requestConfirmation
    QString m_requestedPasskey;

    // requestAuthorization
    bool m_authorizationRequested;

    // authorizeService
    QString m_authorizedUuid;

    // cancel
    bool m_cancelCalled;

    // release
    bool m_releaseCalled;
};

class AgentManagerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void requestPinCodeTest();
    void displayPinCodeTest();
    void requestPasskeyTest();
    void displayPasskeyTest();
    void requestConfirmationTest();
    void requestAuthorizationTest();
    void authorizeServiceTest();
    void cancelTest();
    void releaseTest();

private:
    TestAgent *m_agent;
    QDBusObjectPath m_device;
};

#endif // AGENTMANAGERTEST_H
