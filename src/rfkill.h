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

#ifndef BLUEZQT_RFKILL_H
#define BLUEZQT_RFKILL_H

#include <QObject>

namespace BluezQt
{

class Rfkill : public QObject
{
    Q_OBJECT

public:
    enum State {
        Unblocked,
        SoftBlocked,
        HardBlocked,
        Unknown
    };

    explicit Rfkill(QObject *parent = Q_NULLPTR);
    ~Rfkill();

    State state() const;
    bool block();
    bool unblock();

Q_SIGNALS:
    void stateChanged(State state);

private Q_SLOTS:
    void devReadyRead();

private:
    void init();
    bool openForWriting();
    State readState() const;
    bool setSoftBlock(quint8 soft);

    int m_readFd;
    int m_writeFd;
    State m_state;
};

} // namespace BluezQt

#endif // BLUEZQT_RFKILL_H
