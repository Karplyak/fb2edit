#ifndef FB2TEMP_H
#define FB2TEMP_H

#include <QByteArray>
#include <QList>
#include <QNetworkReply>
#include <QString>
#include <QTemporaryFile>
#include <QNetworkAccessManager>

class Fb2TextEdit;

class Fb2TemporaryFile : public QTemporaryFile
{
    Q_OBJECT
public:
    static QString md5(const QByteArray &data);
public:
    explicit Fb2TemporaryFile(const QString &name);
    inline qint64 write(const QByteArray &data);
    void setHash(const QString &hash) { m_hash = hash; }
    const QString & hash() const { return m_hash; }
    const QString & name() const { return m_name; }
    QByteArray data();
private:
    const QString m_name;
    QString m_hash;
};

class Fb2TemporaryList : public QList<Fb2TemporaryFile*>
{
public:
    explicit Fb2TemporaryList();
    virtual ~Fb2TemporaryList();

    QString add(const QString &path, const QByteArray &data);
    bool exists(const QString &name) const;
    Fb2TemporaryFile * get(const QString &name) const;
    const QString & set(const QString &name, const QByteArray &data, const QString &hash = QString());
    QString name(const QString &hash) const;
    QByteArray data(const QString &name) const;
private:
    QString newName(const QString &path);
};

typedef QListIterator<Fb2TemporaryFile*> Fb2TemporaryIterator;

#if 0

class Fb2NetworkDiskCache : public QNetworkDiskCache
{
public:
    explicit Fb2NetworkDiskCache(QObject *parent = 0) : QNetworkDiskCache(parent) {}
    QNetworkCacheMetaData metaData(const QUrl &url);
    QIODevice *data(const QUrl &url);
};

#endif

class Fb2ImageReply : public QNetworkReply
{
    Q_OBJECT
public:
    explicit Fb2ImageReply(QNetworkAccessManager::Operation op, const QNetworkRequest &request, const QByteArray &data);
    qint64 bytesAvailable() const { return content.size(); }
    bool isSequential() const { return true; }
    void abort() { close(); }

protected:
    qint64 readData(char *data, qint64 maxSize);

private:
    QByteArray content;
    qint64 offset;
};

class Fb2NetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit Fb2NetworkAccessManager(Fb2TextEdit &view);

protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0);

private:
    QNetworkReply *imageRequest(Operation op, const QNetworkRequest &request);

private:
    Fb2TextEdit & m_view;
    QString m_path;
};

#endif // FB2TEMP_H
