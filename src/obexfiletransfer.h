#ifndef QBLUEZ_OBEXFILETRANSFER_H
#define QBLUEZ_OBEXFILETRANSFER_H

#include <QObject>
#include <QDateTime>

#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class PendingCall;

class QBLUEZ_EXPORT ObexFileTransfer : public QObject
{
    Q_OBJECT

public:
    struct Item
    {
        enum Type { File, Folder };

        QString name;
        Type type;
        quint64 size;
        QString permissions;
        QDateTime modified;
        QDateTime accessed;
        QDateTime created;
    };

    explicit ObexFileTransfer(const QDBusObjectPath &path, QObject *parent = 0);
    ~ObexFileTransfer();

    QDBusObjectPath objectPath() const;

    // Possible errors: InvalidArguments, Failed
    PendingCall *changeFolder(const QString &folder);

    // Possible errors: InvalidArguments, Failed
    PendingCall *createFolder(const QString &folder);

    // Possible errors: Failed
    // Return: QList<ObexFileTransfer::Item>
    PendingCall *listFolder();

    // Possible errors: InvalidArguments, Failed
    // Return: ObexTransfer*
    PendingCall *getFile(const QString &targetFileName, const QString &sourceFileName);

    // Possible errors: InvalidArguments, Failed
    // Return: ObexTransfer*
    PendingCall *putFile(const QString &sourceFileName, const QString &targetFileName);

    // Possible errors: InvalidArguments, Failed
    PendingCall *copyFile(const QString &sourceFileName, const QString &targetFileName);

    // Possible errors: InvalidArguments, Failed
    PendingCall *moveFile(const QString &sourceFileName, const QString &targetFileName);

    // Possible errors: InvalidArguments, Failed
    PendingCall *deleteFile(const QString &fileName);

private:
    class ObexFileTransferPrivate *const d;

    friend class ObexFileTransferPrivate;
};

} // namespace QBluez

#endif // QBLUEZ_OBEXFILETRANSFER_H
