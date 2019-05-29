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

#include "Comment.h"

class Property
{
public:
    struct Tags {
        bool isOptional = false;
        bool isExperimental = false;
        bool isReadOnly    = false;
        bool isServerOnly   = false;
    };

    Property();

    bool finalize();

    QString      name() const;
    QString      type() const;
    Tags         tags() const;
    QStringList  comment() const;

private:
    QString     m_name;
    QString     m_type;
    QStringList m_stringTags;
    QString     m_limitation;
    Comment     m_comment;

    // finalized members
    Tags    m_tags;

    friend class Properties;
};
