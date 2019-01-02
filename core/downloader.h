#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>

class QFile;
class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    ~Downloader();

    void httpDownload(const QUrl &url, const QString &strDir);

signals:
    void sigDownloadFinished(const QString &strFilePath);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void replyFinished(QNetworkReply *reply);
    void onReadyRead();

private:
    QFile *file;
    QNetworkAccessManager *accessManager;
    QNetworkReply *reply;
    QNetworkRequest *request;
};

#endif // DOWNLOADER_H
