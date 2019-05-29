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

#include "Properties.h"

#include <QRegExp>
#include <QStringList>

Properties::Properties()
{
}

void Properties::parse(const QString &line)
{
    QRegExp rx(QStringLiteral(
               "(?:Properties|^)"   // Properties keyword or start of line
               "\\t{1,2}"           // preceding tabs (max 2)
               "([a-z1-6{}_]+)"     // type name
               " "                  // space
               "([A-Z]\\w+)"        // method name
               "(?: \\[(.*)\\])?"   // tags
               "(?: \\((.*)\\))?"   // limitations
               ), Qt::CaseSensitive, QRegExp::RegExp2);

    // Check if we match a property
    if (rx.indexIn(line) != -1) {
        QStringList list = rx.capturedTexts();
        m_properties.emplace_back(Property());
        m_currentProperty = &m_properties.back();
        m_currentProperty->m_type = list.at(1).toLower();
        m_currentProperty->m_name = list.at(2);
        m_currentProperty->m_stringTags = list.at(3).toLower().split(QStringLiteral(", "), QString::SkipEmptyParts);
        m_currentProperty->m_limitation = list.at(4).toLower();
    } else if (m_currentProperty) {
        // Skip first empty line
        if (line.isEmpty() && m_currentProperty->m_comment.isEmpty()) {
            return;
        }
        m_currentProperty->m_comment.append(line);
    }
}

bool Properties::finalize()
{
    bool success = true;

    for (auto &property : m_properties) {
        success &= property.finalize();
    }

    return success;
}


std::list<Property> Properties::properties() const
{
    return m_properties;
}
