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

#include "Method.h"

Method::Method()
{
}

bool Method::finalize()
{
    for (const auto& tag : m_stringTags) {
        m_tags.isOptional |= tag.contains("optional", Qt::CaseInsensitive);
        m_tags.isDeprecated |= tag.contains("deprecated", Qt::CaseInsensitive);
        m_tags.isExperimental |= tag.contains("experimental", Qt::CaseInsensitive);
    }

    m_outParameters.removeOne("void");
    if (m_outParameters.isEmpty()) {
        return true;
    }

    // Guess out parameter name from method name
    QString paramName = guessOutParameterName();
    if (!paramName.isEmpty()) {
        m_outParameters.front() += " " + paramName;
        return true;
    }

    for (int i = 0; i < m_outParameters.size(); ++i) {
        m_outParameters[i] += " value" + QString::number(i);
    }

    return true;
}

const QString& Method::name() const
{
    return m_name;
}

const QStringList& Method::inParameters() const
{
    return m_inParameters;
}

const QStringList& Method::outParameters() const
{
    return m_outParameters;
}

const Method::Tags& Method::tags() const
{
    return m_tags;
}

const QStringList& Method::comment() const
{
    return m_comment;
}

QString Method::guessOutParameterName() const
{
    if (m_outParameters.size() != 1) {
        return QString();
    }

    QRegExp rx("([A-Z][a-z0-9]+)+");
    if (rx.indexIn(m_name, 1) == -1) {
        return QString("value");
    }

    QStringList list = rx.capturedTexts();
    return list.last().toLower();
}
