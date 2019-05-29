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

#include "Property.h"

Property::Property()
{
}

bool Property::finalize()
{
    for (auto tag : m_stringTags) {
        m_tags.isOptional |= tag.contains(QStringLiteral("optional"), Qt::CaseInsensitive);
        m_tags.isExperimental |= tag.contains(QStringLiteral("experimental"), Qt::CaseInsensitive);
        m_tags.isReadOnly |= tag.contains(QStringLiteral("read-only"), Qt::CaseInsensitive);
    }
    m_tags.isServerOnly = m_limitation.contains(QStringLiteral("server only"), Qt::CaseInsensitive);

    bool success = true;
    success &= m_comment.finalize();

    return success;
}

QString Property::name() const
{
    return m_name;
}

QString Property::type() const
{
    return m_type;
}

Property::Tags Property::tags() const
{
    return m_tags;
}

QStringList Property::comment() const
{
    return m_comment;
}

