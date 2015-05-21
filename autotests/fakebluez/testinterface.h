/*
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

#ifndef TESTINTERFACE_H
#define TESTINTERFACE_H

#include <QDBusMessage>
#include <QDBusAbstractAdaptor>

class FakeBluez;

class TestInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.bluezqt.fakebluez.Test")

public:
    explicit TestInterface(FakeBluez *parent);

public Q_SLOTS:
    void runTest(const QString &testName);
    void runAction(const QString &object, const QString &actionName, const QVariantMap &properties, const QDBusMessage &msg);

    void emitActionFinished();

private:
    FakeBluez *m_fakeBluez;
    QDBusMessage m_msg;
};

#endif // TESTINTERFACE_H
