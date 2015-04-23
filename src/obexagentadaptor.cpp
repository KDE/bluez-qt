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

#include "obexagentadaptor.h"
#include "obexagent.h"
#include "obexmanager.h"
#include "obextransfer.h"
#include "obextransfer_p.h"
#include "dbusproperties.h"
#include "utils.h"

#include <QDBusObjectPath>

namespace BluezQt
{

typedef org::freedesktop::DBus::Properties DBusProperties;

ObexAgentAdaptor::ObexAgentAdaptor(ObexAgent *parent, ObexManager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
{
}

QString ObexAgentAdaptor::AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_transferRequest = Request<QString>(OrgBluezObexAgent, msg);
    m_transferPath = transfer.path();

    DBusProperties dbusProperties(Strings::orgBluezObex(), m_transferPath, DBusConnection::orgBluezObex(), this);

    const QDBusPendingReply<QVariantMap> &call = dbusProperties.GetAll(Strings::orgBluezObexTransfer1());
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, &ObexAgentAdaptor::getPropertiesFinished);

    return QString();
}

void ObexAgentAdaptor::Cancel()
{
    m_agent->cancel();
}

void ObexAgentAdaptor::Release()
{
    m_agent->release();
}

void ObexAgentAdaptor::getPropertiesFinished(QDBusPendingCallWatcher *watcher)
{
    const QDBusPendingReply<QVariantMap> &reply = *watcher;
    watcher->deleteLater();

    if (reply.isError()) {
        m_transferRequest.cancel();
        return;
    }

    ObexTransferPtr transfer = ObexTransferPtr(new ObexTransfer(m_transferPath, reply.value()));
    transfer->d->q = transfer.toWeakRef();

    ObexSessionPtr session = m_manager->sessionForPath(transfer->objectPath());
    Q_ASSERT(session);

    if (!session) {
        m_transferRequest.cancel();
        return;
    }

    m_agent->authorizePush(transfer, session, m_transferRequest);
}

} // namespace BluezQt
