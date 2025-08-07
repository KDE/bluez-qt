/*
 * BluezQt - Asynchronous Bluez wrapper library
 *
 * SPDX-FileCopyrightText: 2015 David Rosca <nowrep@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef BLUEZQT_RFKILL_P_H
#define BLUEZQT_RFKILL_P_H

#include <QHash>
#include <QMutex>
#include <QObject>
#include <QThread>
#include <QWaitCondition>

#include "bluezqt_export.h"
#include "rfkill.h"

namespace BluezQt
{

class RfkillThread : public QThread
{
    Q_OBJECT

public:
    explicit RfkillThread(QObject *parent);

    void interrupt();
    void setSoftBlocked(quint8 blocked);

protected:
    void run() override;

private:
    void doSetSoftBlocked(quint8 blocked);

    QWaitCondition m_waitCondition;
    QMutex m_mutex;
    quint8 m_pendingBlocked;
};

struct RfkillPrivate {
    int m_readFd = -1;
    Rfkill::State m_state = Rfkill::State::Unknown;
    QHash<quint32, Rfkill::State> m_devices;
    RfkillThread *m_thread = nullptr;
};

} // namespace BluezQt

#endif // BLUEZQT_RFKILL_P_H
