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

#ifndef BLUEZQT_OBEXTRANSFER_P_H
#define BLUEZQT_OBEXTRANSFER_P_H

#include "obextransfer.h"

#include "obextransfer1.h"
#include "dbusproperties.h"

namespace BluezQt
{

typedef org::bluez::obex::Transfer1 BluezTransfer;
typedef org::freedesktop::DBus::Properties DBusProperties;

class ObexTransferPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ObexTransferPrivate(const QString &path, const QVariantMap &properties);

    void init(const QVariantMap &properties);

    void propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);
    void sessionRemoved(const ObexSessionPtr &session);

    QWeakPointer<ObexTransfer> q;
    BluezTransfer *m_bluezTransfer;
    DBusProperties *m_dbusProperties;

    ObexTransfer::Status m_status;
    QString m_name;
    QString m_type;
    quint64 m_time;
    quint64 m_size;
    quint64 m_transferred;
    QString m_fileName;
    bool m_suspendable;
};

} // namespace BluezQt

#endif // BLUEZQT_OBEXTRANSFER_P_H
