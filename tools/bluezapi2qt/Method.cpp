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
    for (const auto &tag : m_stringTags) {
        m_tags.isOptional |= tag.contains(QStringLiteral("optional"), Qt::CaseInsensitive);
        m_tags.isDeprecated |= tag.contains(QStringLiteral("deprecated"), Qt::CaseInsensitive);
        m_tags.isExperimental |= tag.contains(QStringLiteral("experimental"), Qt::CaseInsensitive);
    }

    bool success = true;
    success &= m_comment.finalize();

    for (const auto &inParam : m_inParameterStrings) {
        m_inParameters.push_back(Parameter::fromString(inParam));
    }

    m_outParameterStrings.removeOne(QStringLiteral("void"));
    if (m_outParameterStrings.isEmpty()) {
        m_outParameter = Parameter::fromString(QStringLiteral("void unnamend"));
        return true;
    }

    // Guess out parameter name from method name
    QString paramName = guessOutParameterName();
    if (!paramName.isEmpty()) {
        m_outParameterStrings.front() += QStringLiteral(" ") + paramName;
        m_outParameter = Parameter::fromString(m_outParameterStrings.front());
    } else {
        for (int i = 0; i < m_outParameterStrings.size(); ++i) {
            m_outParameterStrings[i] += QStringLiteral(" value") + QString::number(i);
        }
    }

    for (const auto &outParam : m_outParameterStrings) {
        m_outParameters.push_back(Parameter::fromString(outParam));
    }

    return success;
}

QString Method::name() const
{
    return m_name;
}

QList<Parameter> Method::inParameters() const
{
    return m_inParameters;
}

QList<Parameter> Method::outParameters() const
{
    return m_outParameters;
}

Parameter Method::outParameter() const
{
    return m_outParameter;
}

Method::Tags Method::tags() const
{
    return m_tags;
}

QStringList Method::comment() const
{
    return m_comment;
}

QString Method::guessOutParameterName() const
{
    if (m_outParameterStrings.size() != 1) {
        return QString();
    }

    QRegExp rx(QStringLiteral("([A-Z][a-z0-9]+)+"));
    if (rx.indexIn(m_name, 1) == -1) {
        return QStringLiteral("value");
    }

    QStringList list = rx.capturedTexts();
    return list.last().toLower();
}
