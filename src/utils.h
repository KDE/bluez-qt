/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2014-2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_UTILS_H
#define BLUEZQT_UTILS_H

#include "device.h"

#include <QStringList>

class QString;
class QDBusConnection;

namespace BluezQt
{
namespace Strings
{
QString orgFreedesktopDBus();
QString orgFreedesktopDBusProperties();
QString orgBluez();
QString orgBluezAdapter1();
QString orgBluezBattery1();
QString orgBluezDevice1();
QString orgBluezInput1();
QString orgBluezGattManager1();
QString orgBluezLEAdvertisingManager1();
QString orgBluezMedia1();
QString orgBluezMediaPlayer1();
QString orgBluezMediaTransport1();
QString orgBluezAgentManager1();
QString orgBluezProfileManager1();
QString orgBluezObex();
QString orgBluezObexClient1();
QString orgBluezObexAgentManager1();
QString orgBluezObexSession1();
QString orgBluezObexTransfer1();

}

namespace DBusConnection
{
QDBusConnection orgBluez();
QDBusConnection orgBluezObex();

}

namespace Instance
{
Manager *manager();
void setManager(Manager *manager);

ObexManager *obexManager();
void setObexManager(ObexManager *obexManager);

}

QStringList stringListToUpper(const QStringList &list);
Device::Type classToType(quint32 classNum);
Device::Type appearanceToType(quint16 appearance);

} // namespace BluezQt

#endif // BLUEZQT_UTILS_H
