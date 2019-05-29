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

#ifndef INTERFACE_H
#define INTERFACE_H

#include "Methods.h"
#include "Properties.h"

class Interface
{
public:
    Interface();

    bool    parse(const QString &line);
    bool    finalize();

    QStringList comment() const;
    QString     service() const;
    QString     name() const;
    QString     objectPath() const;
    Methods     methods() const;
    Properties  properties() const;

private:
    enum class State {
        Comment,
        Service,
        Interface,
        ObjectPath,
        Methods,
        Properties
    };

    void    parseComment(const QString &line);
    void    parseService(const QString &line);
    void    parseInterface(const QString &line);
    void    parseObjectPath(const QString &line);

    State   m_state = State::Comment;

    QStringList m_comment;
    QString     m_service;
    QString     m_name;
    QString     m_objectPath;
    Methods     m_methods;
    Properties  m_properties;
};

#endif // INTERFACE_H
