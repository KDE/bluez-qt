/*
 * BluezQt - Asynchronous BlueZ wrapper library
 *
 * Copyright (C) 2019 Manuel Weichselbaumer <mincequi@web.de>
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

#pragma once

#include <QDBusPendingReply>

#include "pendingcall.h"

namespace BluezQt
{

using namespace std::placeholders;

/**
 * @class BluezQt::TPendingCall tpendingcall.h <BluezQt/TPendingCall>
 *
 * Pending method call (template version).
 *
 * This class represents a pending method call. It is a convenient wrapper
 * around QDBusPendingReply and QDBusPendingCallWatcher.
 * The TPendingCall is a template class whose parameters are the types that will
 * be used to extract the contents of the reply's data.
 */

// KF6 TODO: convert all PendingCalls to TPendingCall (or convert existing PendingCall class to templated version).
template<class...T>
class TPendingCall : public PendingCall
{
private:
    template<int Index, typename Ty, typename ...Ts>
    struct Select
    {
        using Type = typename Select<Index-1, Ts...>::Type;
    };
    template<typename Ty, typename ...Ts>
    struct Select<0, Ty, Ts...>
    {
        using Type = Ty;
    };

public:
    /**
     * Returns a return value at given index of the call.
     *
     * Returns the return value at position Index (which is a template parameter) cast to type Type.
     * This function uses template code to determine the proper Type type, according to the type
     * list used in the construction of this object.
     *
     * @return return value at index
     */
    template<int Index> inline
    const typename Select<Index, T...>::Type valueAt() const
    {
        using ResultType = typename Select<Index, T...>::Type;
        return qdbus_cast<ResultType>(m_reply.argumentAt(Index), 0);
    }

private:
    TPendingCall(const QDBusPendingCall &call, QObject *parent = nullptr)
        : PendingCall(call, std::bind(&TPendingCall::process, this, _1, _2, _3), parent)
    {
    }

    void process(QDBusPendingCallWatcher *watcher, ErrorProcessor errorProcessor, QVariantList *values)
    {
        m_reply = *watcher;
        errorProcessor(m_reply.error());
        if (m_reply.isError()) {
            return;
        }

        for (int i = 0; i < m_reply.count(); ++i) {
            values->append(m_reply.argumentAt(i));
        }
    }

    QDBusPendingReply<T...> m_reply;

    friend class MediaTransport;
};

} // namespace BluezQt
