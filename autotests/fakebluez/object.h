#ifndef OBJECT_H
#define OBJECT_H

#include <QVariant>
#include <QDBusObjectPath>

class Object
{
public:
    explicit Object();

    QDBusObjectPath path() const;
    void setPath(const QDBusObjectPath &path);

    QString name() const;
    void setName(const QString &name);

    bool haveProperties() const;

    QVariantMap properties() const;
    void setProperties(const QVariantMap &properties);

    QVariant property(const QString &name) const;
    void changeProperty(const QString &name, const QVariant &value);

private:
    QDBusObjectPath m_path;
    QString m_name;
    QVariantMap m_properties;
};

#endif // OBJECT_H
