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

#ifndef FAKEBLUEZ_H
#define FAKEBLUEZ_H

#include <QObject>
#include <QVariantMap>

class TestInterface;
class ObjectManager;
class AgentManager;
class ProfileManager;
class DeviceManager;
class ObexObject;
class ObexAgentManager;
class ObexClient;

class FakeBluez : public QObject
{
    Q_OBJECT

public:
    explicit FakeBluez(QObject *parent = Q_NULLPTR);

    void runTest(const QString &testName);
    void runAction(const QString &object, const QString &actionName, const QVariantMap &properties);

private Q_SLOTS:
    void doRunAction();

private:
    void clear();
    void createObjectManager();
    void createAgentManager();
    void createProfileManager();
    void createDeviceManager();
    void createObexObjectManager();
    void createObexAgentManager();
    void createObexClient();

    void runBluezNotExportingInterfacesTest();
    void runBluezEmptyManagedObjectsTest();
    void runBluezNoAdaptersTest();
    void runBluezStandardTest();

    void runObexNotExportingInterfacesTest();
    void runObexEmptyManagedObjectsTest();
    void runObexNoClientTest();
    void runObexNoAgentManagerTest();
    void runObexStandardTest();

    TestInterface *m_testInterface;
    ObjectManager *m_objectManager;
    AgentManager *m_agentManager;
    ProfileManager *m_profileManager;
    DeviceManager *m_deviceManager;

    ObexObject *m_obexObject;
    ObexAgentManager *m_obexAgentManager;
    ObexClient *m_obexClient;

    QString m_actionObject;
    QString m_actionName;
    QVariantMap m_actionProperties;
};

#endif // FAKEBLUEZ_H
