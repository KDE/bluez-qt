/*
 * QBluez - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2014 David Rosca <nowrep@gmail.com>
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

#include <QDBusObjectPath>

namespace QBluez
{

ObexAgentAdaptor::ObexAgentAdaptor(ObexAgent *parent, ObexManager *manager)
    : QDBusAbstractAdaptor(parent)
    , m_agent(parent)
    , m_manager(manager)
    , m_transfer(0)
{
}

QString ObexAgentAdaptor::AuthorizePush(const QDBusObjectPath &transfer, const QDBusMessage &msg)
{
    msg.setDelayedReply(true);
    m_transferRequest = Request<QString>(OrgBluezObexAgent, msg);

    m_transfer = new ObexTransfer(transfer.path(), this);
    m_transfer->d->init();
    connect(m_transfer->d, &ObexTransferPrivate::initFinished, this, &ObexAgentAdaptor::transferInitFinished);
    connect(m_transfer->d, &ObexTransferPrivate::initError, this, &ObexAgentAdaptor::transferInitError);

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

void ObexAgentAdaptor::transferInitFinished()
{
    m_agent->authorizePush(m_transfer, m_transferRequest);
}

void ObexAgentAdaptor::transferInitError()
{
    m_transfer->deleteLater();
    m_transferRequest.cancel();
}

} // namespace QBluez
