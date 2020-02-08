/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "rfkill.h"
#include "debug.h"

#ifdef Q_OS_LINUX
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#endif

#include <QSocketNotifier>

namespace BluezQt
{

#ifdef Q_OS_LINUX
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
    quint32 idx;
    quint8 type;
    quint8 op;
    quint8 soft;
    quint8 hard;
};
#endif

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
#ifdef Q_OS_LINUX
    if (m_readFd != -1) {
        ::close(m_readFd);
    }

    if (m_writeFd != -1) {
        ::close(m_writeFd);
    }
#endif
}

Rfkill::State Rfkill::state() const
{
    return m_state;
}

bool Rfkill::block()
{
    if (m_state == SoftBlocked || m_state == HardBlocked) {
        return true;
    }

    if (m_state != Unblocked) {
        return false;
    }

    return setSoftBlock(1);
}

bool Rfkill::unblock()
{
    if (m_state == Unblocked) {
        return true;
    }

    if (m_state != SoftBlocked) {
        return false;
    }

    return setSoftBlock(0);
}

void Rfkill::devReadyRead()
{
    State oldState = m_state;

    updateRfkillDevices();

    if (m_state != oldState) {
        Q_EMIT stateChanged(m_state);
    }
}

void Rfkill::init()
{
#ifdef Q_OS_LINUX
    m_readFd = ::open("/dev/rfkill", O_RDONLY | O_CLOEXEC);

    if (m_readFd == -1) {
        qCWarning(BLUEZQT) << "Cannot open /dev/rfkill for reading!";
        return;
    }

    if (::fcntl(m_readFd, F_SETFL, O_NONBLOCK) < 0) {
        ::close(m_readFd);
        m_readFd = -1;
        return;
    }

    updateRfkillDevices();

    QSocketNotifier *notifier = new QSocketNotifier(m_readFd, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &Rfkill::devReadyRead);
#endif
}

bool Rfkill::openForWriting()
{
#ifndef Q_OS_LINUX
    return false;
#else
    if (m_writeFd != -1) {
        return true;
    }

    m_writeFd = ::open("/dev/rfkill", O_WRONLY | O_CLOEXEC);

    if (m_writeFd == -1) {
        qCWarning(BLUEZQT) << "Cannot open /dev/rfkill for writing!";
        return false;
    }

    if (::fcntl(m_writeFd, F_SETFL, O_NONBLOCK) < 0) {
        ::close(m_writeFd);
        m_writeFd = -1;
        return false;
    }

    return true;
#endif
}

#ifdef Q_OS_LINUX
static Rfkill::State getState(const rfkill_event &event)
{
    if (event.hard) {
        return Rfkill::HardBlocked;
    } else if (event.soft) {
        return Rfkill::SoftBlocked;
    }
    return Rfkill::Unblocked;
}
#endif

void Rfkill::updateRfkillDevices()
{
#ifdef Q_OS_LINUX
    if (m_readFd == -1) {
        return;
    }

    rfkill_event event;
    while (::read(m_readFd, &event, sizeof(event)) == sizeof(event)) {
        if (event.type != RFKILL_TYPE_BLUETOOTH) {
            continue;
        }

        switch (event.op) {
        case RFKILL_OP_ADD:
        case RFKILL_OP_CHANGE:
            m_devices[event.idx] = getState(event);
            break;

        case RFKILL_OP_DEL:
            m_devices.remove(event.idx);
            break;

        case RFKILL_OP_CHANGE_ALL:
            for (auto it = m_devices.begin(); it != m_devices.end(); ++it) {
                it.value() = getState(event);
            }
            break;

        default:
            break;
        }
    }

    // Update global state
    m_state = Unknown;

    for (State state : qAsConst(m_devices)) {
        Q_ASSERT(state != Unknown);

        if (m_state == Unknown) {
            m_state = state;
        } else if (state > m_state) {
            m_state = state;
        }
    }

    qCDebug(BLUEZQT) << "Rfkill global state changed:" << m_state;
#endif
}

bool Rfkill::setSoftBlock(quint8 soft)
{
#ifndef Q_OS_LINUX
    Q_UNUSED(soft)
    return false;
#else
    if (!openForWriting()) {
        return false;
    }

    rfkill_event event;
    ::memset(&event, 0, sizeof(event));
    event.op = RFKILL_OP_CHANGE_ALL;
    event.type = RFKILL_TYPE_BLUETOOTH;
    event.soft = soft;

    bool ret = ::write(m_writeFd, &event, sizeof(event)) == sizeof(event);
    qCDebug(BLUEZQT) << "Setting Rfkill soft block succeeded:" << ret;
    return ret;
#endif
}

} // namespace BluezQt
