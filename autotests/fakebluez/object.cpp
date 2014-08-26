#include "object.h"

#include <QDBusAbstractAdaptor>

Object::Object()
{
}

QDBusObjectPath Object::path() const
{
    return m_path;
}

void Object::setPath(const QDBusObjectPath &path)
{
    m_path = path;
}

QString Object::name() const
{
    return m_name;
}

void Object::setName(const QString &name)
{
    m_name = name;
}

bool Object::haveProperties() const
{
    return !m_properties.isEmpty();
}

QVariantMap Object::properties() const
{
    return m_properties;
}

void Object::setProperties(const QVariantMap &properties)
{
    m_properties = properties;
}

QVariant Object::property(const QString &name) const
{
    return m_properties.value(name);
}

void Object::setProperty(const QString &name, const QVariant &value)
{
    m_properties[name] = value;
}
