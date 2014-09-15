#ifndef QBLUEZ_OBEXFILETRANSFER_H
#define QBLUEZ_OBEXFILETRANSFER_H

#include <QObject>
#include <QDateTime>

#include "qbluez_export.h"

class QDBusObjectPath;

namespace QBluez
{

class PendingCall;

/**
 * OBEX file transfer.
 *
 * This class represents an OBEX file transfer interface.
 */
class QBLUEZ_EXPORT ObexFileTransfer : public QObject
{
    Q_OBJECT

public:
    /** Item returned from ObexFileTransfer::listFolder(). */
    struct Item
    {
        /** Type of item. */
        enum Type {
            /** Indicates that the item is a file. */
            File,
            /** Indicates that the item is a folder. */
            Folder
        };

        /** Name of the file/folder. */
        QString name;
        /** Label of the file/folder. */
        QString label;
        /** Type of the item. */
        Type type;
        /** Size of file in bytes or number of items in folder. */
        quint64 size;
        /** Permissions of the item. */
        QString permissions;
        /** Memory type where the item is stored. */
        QString memoryType;
        /** Modification time of the item. */
        QDateTime modified;
    };

    /**
     * Creates a new ObexFileTransfer object.
     *
     * This class will be typically used with a @p path
     * from result of ObexManager::createSession().
     *
     * @param path path of session
     * @param parent
     */
    explicit ObexFileTransfer(const QDBusObjectPath &path, QObject *parent = 0);

    /**
     * Destroys an ObexFileTransfer object.
     */
    ~ObexFileTransfer();

    /**
     * D-Bus object path of the file transfer session.
     *
     * @return object path of session
     */
    QDBusObjectPath objectPath() const;

    /**
     * Changes the current folder.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param folder folder to be changed
     * @return void pending call
     */
    PendingCall *changeFolder(const QString &folder);

    /**
     * Creates a new folder.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param folder name of new folder
     * @return void pending call
     */
    PendingCall *createFolder(const QString &folder);

    /**
     * Lists a current folder.
     *
     * Possible errors: PendingCall::Failed
     *
     * @return QList<ObexFileTransfer::Item> pending call
     */
    PendingCall *listFolder();

    /**
     * Gets the file from the remote device.
     *
     * If an empty @p targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param targetFileName full local path where the file will be saved
     * @param sourceFileName file within the remote device
     * @return ObexTransfer * pending call
     */
    PendingCall *getFile(const QString &targetFileName, const QString &sourceFileName);

    /**
     * Puts the file to the remote device.
     *
     * If an empty @p targetFileName is given, a name will be
     * automatically calculated for the temporary file.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param sourceFileName full path of the local file
     * @param targetFileName file to be saved within the remote device
     * @return ObexTransfer * pending call
     */
    PendingCall *putFile(const QString &sourceFileName, const QString &targetFileName);

    /**
     * Copies a file within the remote device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param sourceFileName source within the remote device
     * @param targetFileName target file within the remote device
     * @return void pending call
     */
    PendingCall *copyFile(const QString &sourceFileName, const QString &targetFileName);

    /**
     * Moves a file within the remote device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param sourceFileName source file within the remote device
     * @param targetFileName target file within the remote device
     * @return void pending call
     */
    PendingCall *moveFile(const QString &sourceFileName, const QString &targetFileName);

    /**
     * Deletes a file/folder within the remote device.
     *
     * Possible errors: PendingCall::InvalidArguments, PendingCall::Failed
     *
     * @param fileName file within the remote device
     * @return void pending call
     */
    PendingCall *deleteFile(const QString &fileName);

private:
    class ObexFileTransferPrivate *const d;

    friend class ObexFileTransferPrivate;
};

} // namespace QBluez

Q_DECLARE_METATYPE(QList<QBluez::ObexFileTransfer::Item>)

#endif // QBLUEZ_OBEXFILETRANSFER_H
