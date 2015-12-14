/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
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

#include "utils.h"
#include "manager.h"
#include "obexmanager.h"
#include "bluezqt_export.h"

#include <QPointer>
#include <QStringList>
#include <QDBusConnection>

namespace BluezQt
{

class GlobalData
{
public:
    explicit GlobalData();

    bool testRun;
    QString orgFreedesktopDBus;
    QString orgBluez;
    QString orgBluezAdapter1;
    QString orgBluezDevice1;
    QString orgBluezInput1;
    QString orgBluezMediaPlayer1;
    QString orgBluezAgentManager1;
    QString orgBluezProfileManager1;
    QString orgBluezObex;
    QString orgBluezObexClient1;
    QString orgBluezObexAgentManager1;
    QString orgBluezObexSession1;
    QString orgBluezObexTransfer1;
    QPointer<Manager> manager;
    QPointer<ObexManager> obexManager;
};

GlobalData::GlobalData()
{
    testRun = false;
    orgFreedesktopDBus = QStringLiteral("org.freedesktop.DBus");
    orgBluez = QStringLiteral("org.bluez");
    orgBluezAdapter1 = QStringLiteral("org.bluez.Adapter1");
    orgBluezDevice1 = QStringLiteral("org.bluez.Device1");
    orgBluezInput1 = QStringLiteral("org.bluez.Input1");
    orgBluezMediaPlayer1 = QStringLiteral("org.bluez.MediaPlayer1");
    orgBluezAgentManager1 = QStringLiteral("org.bluez.AgentManager1");
    orgBluezProfileManager1 = QStringLiteral("org.bluez.ProfileManager1");
    orgBluezObex = QStringLiteral("org.bluez.obex");
    orgBluezObexClient1 = QStringLiteral("org.bluez.obex.Client1");
    orgBluezObexAgentManager1 = QStringLiteral("org.bluez.obex.AgentManager1");
    orgBluezObexSession1 = QStringLiteral("org.bluez.obex.Session1");
    orgBluezObexTransfer1 = QStringLiteral("org.bluez.obex.Transfer1");
}

Q_GLOBAL_STATIC(GlobalData, globalData)

// For fakebluez tests
BLUEZQT_EXPORT void bluezqt_initFakeBluezTestRun()
{
    globalData->testRun = true;
    globalData->orgBluez = QStringLiteral("org.kde.bluezqt.fakebluez");
    globalData->orgBluezObex = QStringLiteral("org.kde.bluezqt.fakebluez");
}

QString Strings::orgFreedesktopDBus()
{
    return globalData->orgFreedesktopDBus;
}

QString Strings::orgBluez()
{
    return globalData->orgBluez;
}

QString Strings::orgBluezAdapter1()
{
    return globalData->orgBluezAdapter1;
}

QString Strings::orgBluezDevice1()
{
    return globalData->orgBluezDevice1;
}

QString Strings::orgBluezInput1()
{
    return globalData->orgBluezInput1;
}

QString Strings::orgBluezMediaPlayer1()
{
    return globalData->orgBluezMediaPlayer1;
}

QString Strings::orgBluezAgentManager1()
{
    return globalData->orgBluezAgentManager1;
}

QString Strings::orgBluezProfileManager1()
{
    return globalData->orgBluezProfileManager1;
}

QString Strings::orgBluezObex()
{
    return globalData->orgBluezObex;
}

QString Strings::orgBluezObexClient1()
{
    return globalData->orgBluezObexClient1;
}

QString Strings::orgBluezObexAgentManager1()
{
    return globalData->orgBluezObexAgentManager1;
}

QString Strings::orgBluezObexSession1()
{
    return globalData->orgBluezObexSession1;
}

QString Strings::orgBluezObexTransfer1()
{
    return globalData->orgBluezObexTransfer1;
}

QDBusConnection DBusConnection::orgBluez()
{
    if (globalData->testRun) {
        return QDBusConnection::sessionBus();
    }
    return QDBusConnection::systemBus();
}

QDBusConnection DBusConnection::orgBluezObex()
{
    return QDBusConnection::sessionBus();
}

Manager *Instance::manager()
{
    return globalData->manager;
}

void Instance::setManager(Manager *manager)
{
    globalData->manager = manager;
}

ObexManager *Instance::obexManager()
{
    return globalData->obexManager;
}

void Instance::setObexManager(ObexManager *obexManager)
{
    globalData->obexManager = obexManager;
}

QStringList stringListToUpper(const QStringList &list)
{
    QStringList converted;
    converted.reserve(list.size());
    Q_FOREACH (const QString &str, list) {
        converted.append(str.toUpper());
    }
    return converted;
}

Device::Type classToType(quint32 classNum)
{
    switch ((classNum & 0x1f00) >> 8) {
    case 0x01:
        return Device::Computer;
    case 0x02:
        switch ((classNum & 0xfc) >> 2) {
        case 0x04:
            return Device::Modem;
        default:
            return Device::Phone;
        }
    case 0x03:
        return Device::Network;
    case 0x04:
        switch ((classNum & 0xfc) >> 2) {
        case 0x01:
        case 0x02:
            return Device::Headset;
        case 0x06:
            return Device::Headphones;
        default:
            return Device::AudioVideo;
        }
    case 0x05:
        switch ((classNum & 0xc0) >> 6) {
        case 0x00:
            switch ((classNum & 0x1e) >> 2) {
            case 0x01:
            case 0x02:
                return Device::Joypad;
            }
            break;
        case 0x01:
            return Device::Keyboard;
        case 0x02:
            switch ((classNum & 0x1e) >> 2) {
            case 0x05:
                return Device::Tablet;
            default:
                return Device::Mouse;
            }
        }
        return Device::Peripheral;
    case 0x06:
        if (classNum & 0x80) {
            return Device::Printer;
        } else if (classNum & 0x20) {
            return Device::Camera;
        }
        return Device::Imaging;
    case 0x07:
        return Device::Wearable;
    case 0x08:
        return Device::Toy;
    case 0x09:
        return Device::Health;
    default:
        return Device::Uncategorized;
    }
}

} // namespace BluezQt
