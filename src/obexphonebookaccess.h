#ifndef OBEXPHONEBOOKACCESS_H
#define OBEXPHONEBOOKACCESS_H

#include <QObject>

#include "types.h"
#include "bluezqt_export.h"

class QDBusObjectPath;

namespace BluezQt {

class BLUEZQT_EXPORT ObexPhonebookAccess : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString folder READ folder)
    Q_PROPERTY(QString databaseIdentifier READ databaseIdentifier)
    Q_PROPERTY(QString primaryCounter READ primaryCounter)
    Q_PROPERTY(QString secondaryCounter READ secondaryCounter)
    Q_PROPERTY(bool fixedImageSize READ fixedImageSize)
public:
    explicit ObexPhonebookAccess(const QString &path, const QVariantMap &properties, QObject *parent = nullptr);
    ~ObexPhonebookAccess();

    /**
     * Returns a shared pointer from this.
     *
     * @return ObexSessionPtr
     */
    ObexPhonebookAccessPtr toSharedPtr() const;

    /**
     * D-Bus object path of the session.
     *
     * @return object path of session
     */
    QDBusObjectPath objectPath() const;

    /**
     * Returns an UBI of the obex phonebook access.
     *
     * Example UBI: ""
     *
     * @return UBI of obex phonebook access
     */
    QString ubi() const;

    QString folder() const;
    QString databaseIdentifier() const;
    QString primaryCounter() const;
    QString secondaryCounter() const;
    bool fixedImageSize();

    PendingCall* select(const QString &location, const QString &phonebook);
    PendingCall* pullAll(const QString &targetFile, const QVariantMap &filters);
    PendingCall* pull(const QString &vcard, const QString &targetfile, const QVariantMap &filters);
    PendingCall* list(const QVariantMap &filters);
    PendingCall* search(const QString &field, const QString &value, const QVariantMap &filters);
    PendingCall* getSize();
    PendingCall* listFilterFields();
    PendingCall* updateVersion();

private:

    class ObexPhonebookAccessPrivate *const d;

    friend class ObexPhonebookAccessPrivate;
};

}
#endif // OBEXPHONEBOOKACCESS_H
