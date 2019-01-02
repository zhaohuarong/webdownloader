#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "accessmanager.h"

AccessManager *AccessManager::m_pAccessManager = NULL;

AccessManager::AccessManager(QObject *parent) :
    QObject(parent)
{
    m_accessManager = new QNetworkAccessManager(this);
    connect(m_accessManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(finishedSlot(QNetworkReply *)));
}

void AccessManager::sync()
{
    int timeout = 30 * 1000;
    QTimer t;
    QEventLoop q;
    t.setSingleShot(true);
    connect(&t, SIGNAL(timeout()), &q, SLOT(quit()));
    connect(this, SIGNAL(sigFinishedSlot()), &q, SLOT(quit()));
    t.start(timeout);
    q.exec();
}

AccessManager *AccessManager::instance()
{
    if(m_pAccessManager == NULL)
        m_pAccessManager = new AccessManager();
    return m_pAccessManager;
}

void AccessManager::getHTML(const QString &url)
{
    QNetworkRequest request;
    request.setUrl(url);
    m_accessManager->get(request);

    sync();
}

void AccessManager::finishedSlot(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        while(1)
        {
            QByteArray bytes = reply->readLine();
            qDebug() << QString::fromUtf8(bytes);
            if(bytes.count() <= 0)
                break;
        }
    }
    else
    {
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
    reply->deleteLater();
    emit sigFinishedSlot();
}
