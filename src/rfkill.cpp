/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * Copyright (C) 2015 David Rosca <nowrep@gmail.com>
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

#include "rfkill.h"
#include "debug_p.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include <QSocketNotifier>

namespace BluezQt
{

enum rfkill_type {
    RFKILL_TYPE_ALL = 0,
    RFKILL_TYPE_WLAN,
    RFKILL_TYPE_BLUETOOTH,
    RFKILL_TYPE_UWB,
    RFKILL_TYPE_WIMAX,
    RFKILL_TYPE_WWAN
};

enum rfkill_operation {
    RFKILL_OP_ADD = 0,
    RFKILL_OP_DEL,
    RFKILL_OP_CHANGE,
    RFKILL_OP_CHANGE_ALL
};

struct rfkill_event {
    uint32_t idx;
    uint8_t type;
    uint8_t op;
    uint8_t soft;
    uint8_t hard;
};

Rfkill::Rfkill(QObject *parent)
    : QObject(parent)
    , m_readFd(-1)
    , m_writeFd(-1)
    , m_state(Unknown)
{
    init();
}

Rfkill::~Rfkill()
{
    if (m_readFd != -1) {
        ::close(m_readFd);
    }

    if (m_writeFd != -1) {
        ::close(m_writeFd);
    }
}

Rfkill::State Rfkill::state() const
{
    return m_state;
}

bool Rfkill::block()
{
    if (m_state != Unblocked) {
        return false;
    }

    return setSoftBlock(1);
}

bool Rfkill::unblock()
{
    if (m_state != SoftBlocked) {
        return false;
    }

    return setSoftBlock(0);
}

void Rfkill::devReadyRead()
{
    State changed = readState();
    if (m_state != changed) {
        m_state = changed;
        Q_EMIT stateChanged(m_state);
    }
}

void Rfkill::init()
{
    m_readFd = ::open("/dev/rfkill", O_RDONLY);

    if (m_readFd == -1) {
        qCDebug(BLUEZQT) << "Cannot open /dev/rfkill for reading!";
        return;
    }

    if (::fcntl(m_readFd, F_SETFL, O_NONBLOCK) < 0) {
        ::close(m_readFd);
        m_readFd = -1;
        return;
    }

    m_state = readState();

    QSocketNotifier *notifier = new QSocketNotifier(m_readFd, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &Rfkill::devReadyRead);
}

bool Rfkill::openForWriting()
{
    if (m_writeFd != -1) {
        return true;
    }

    m_writeFd = ::open("/dev/rfkill", O_WRONLY);

    if (m_writeFd == -1) {
        qCDebug(BLUEZQT) << "Cannot open /dev/rfkill for writing!";
        return false;
    }

    if (::fcntl(m_writeFd, F_SETFL, O_NONBLOCK) < 0) {
        ::close(m_writeFd);
        m_writeFd = -1;
        return false;
    }

    return true;
}

Rfkill::State Rfkill::readState() const
{
    if (m_readFd == -1) {
        return Unknown;
    }

    rfkill_event event;
    while (::read(m_readFd, &event, sizeof(event)) == sizeof(event)) {
        if (event.type != RFKILL_TYPE_BLUETOOTH) {
            continue;
        }

        if (event.soft) {
            return SoftBlocked;
        } else if (event.hard) {
            return HardBlocked;
        } else {
            return Unblocked;
        }
    }

    return Unknown;
}

bool Rfkill::setSoftBlock(quint8 soft)
{
    if (!openForWriting()) {
        return false;
    }

    rfkill_event event;
    ::memset(&event, 0, sizeof(event));
    event.op = RFKILL_OP_CHANGE_ALL;
    event.type = RFKILL_TYPE_BLUETOOTH;
    event.soft = soft;

    return ::write(m_writeFd, &event, sizeof(event)) == sizeof(event);
}

} // namespace BluezQt
