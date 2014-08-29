#include "object.h"

#include <QDBusMessage>
#include <QDBusConnection>
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

void Object::changeProperty(const QString &name, const QVariant &value)
{
    if (m_properties.value(name) == value) {
        return;
    }

    QVariantMap updatedProperties;
    updatedProperties[name] = value;

    m_properties[name] = value;

    QDBusMessage signal = QDBusMessage::createSignal(m_path.path(),
                                                     QStringLiteral("org.freedesktop.DBus.Properties"),
                                                     QStringLiteral("PropertiesChanged"));
    signal << m_name;
    signal << updatedProperties;
    signal << QStringList();
    QDBusConnection::sessionBus().send(signal);
}
