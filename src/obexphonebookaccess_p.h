#ifndef OBEXPHONEBOOKACCESS_P_H
#define OBEXPHONEBOOKACCESS_P_H

#include <QObject>

#include "types.h"
#include "obexphonebookaccess1.h"

namespace BluezQt {

typedef org::bluez::obex::PhonebookAccess1 BluezPhonebookAccess;

class ObexPhonebookAccessPrivate : public QObject
{
    Q_OBJECT

public:
    explicit ObexPhonebookAccessPrivate(const QString &path, const QVariantMap &properties, QObject *parent = nullptr);

    void init(const QVariantMap &properties);

private:
    BluezPhonebookAccess *m_bluezPhonebookAccess;

    QWeakPointer<ObexPhonebookAccess> q;

    QString m_folder;
    QString m_databaseIdentifier;
    QString m_primaryCounter;
    QString m_secondaryCounter;
    bool m_fixedImageSize;

    friend class ObexPhonebookAccess;
};

}

#endif // OBEXPHONEBOOKACCESSPRIVATE_P_H
