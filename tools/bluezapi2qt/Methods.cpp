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

#include "Methods.h"

#include <QStringList>
#include <QRegularExpression>

static const QRegularExpression rx(QStringLiteral(
                             "\\t+"               // preceding tabs
                             "(?:(.+) )?"         // return types - Argh! LE Advertising Manager does not specify return type
                             "([A-Z]\\w+)"        // method name
                             "\\(([^\\)]*)\\)"    // parameters
                             "(?: \\[(.*)\\])?"   // tags
                             "(?: \\((.*)\\))?"   // limitations
                             ), QRegularExpression::CaseInsensitiveOption);

Methods::Methods()
{
}

bool Methods::isMethod(const QString &line)
{
    // Check if we match a method
    return (rx.match(line).hasMatch());
}

void Methods::parse(const QString &line)
{
    // Check if we match a method
    QRegularExpressionMatch match = rx.match(line);
    if (match.hasMatch()) {
        m_methods.emplace_back(Method());
        m_currentMethod = &m_methods.back();
        m_currentMethod->m_outParameterStrings = match.captured(1).toLower().split(QStringLiteral(", "), QString::SkipEmptyParts);
        m_currentMethod->m_name = match.captured(2);
        m_currentMethod->m_inParameterStrings = match.captured(3).split(QStringLiteral(", "), QString::SkipEmptyParts);
        m_currentMethod->m_stringTags = match.captured(4).toLower().split(QStringLiteral(", "), QString::SkipEmptyParts);
        m_currentMethod->m_limitation = match.captured(5).toLower();
    } else if (m_currentMethod) {
        // Skip first empty line
        if (line.isEmpty() && m_currentMethod->m_comment.isEmpty()) {
            return;
        }
        m_currentMethod->m_comment.append(line);
    }
}

bool Methods::finalize()
{
    bool success = true;

    for (auto &method : m_methods) {
        success &= method.finalize();
    }

    return success;
}


std::list<Method> Methods::methods() const
{
    return m_methods;
}
