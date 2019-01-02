#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "downloader.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    request = new QNetworkRequest();
}

Downloader::~Downloader()
{
    delete request;
}

void Downloader::httpDownload(const QUrl &url, const QString &strDir)
{
    if(url.isEmpty())
        return;
    QFileInfo fileInfo = url.path();
    QString strDownloadFile = strDir + fileInfo.fileName();
    file = new QFile(strDownloadFile);
    file->open(QIODevice::WriteOnly);
    accessManager = new QNetworkAccessManager(this);
    request->setUrl(url);

	reply = accessManager->get(*request);

    connect(accessManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SIGNAL(downloadProgress(qint64 ,qint64)));
    connect((QObject *)reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void Downloader::replyFinished(QNetworkReply *reply)
{
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QByteArray bytes = reply->readAll();
    QString result(bytes);
    qDebug() << result << status_code;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        if(file != NULL)
        {
            file->close();
            emit sigDownloadFinished(QFileInfo(*file).absoluteFilePath());
        }
    }
    else
    {
        reply->abort();
    }
    reply->deleteLater();
}

void Downloader::onReadyRead()
{
    file->write(reply->readAll());
}
