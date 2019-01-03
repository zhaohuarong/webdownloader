#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "accessmanager.h"

AccessManager *AccessManager::m_pAccessManager = NULL;

AccessManager::AccessManager(QObject *parent) :
    QObject(parent),
    m_accessManager(NULL),
    m_eCurrentType(TYPE_DEFAULT)
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

void AccessManager::getTitleAndAllPictureUrl(const QString &url, QString &strTitle, QStringList &lstUrl)
{
    m_eCurrentType = TYPE_ALL_PIC_URL;
    m_strTitle.clear();
    m_lstPictureUrl.clear();

    QNetworkRequest request;
    request.setUrl(url);
	m_accessManager->get(request);

    sync();

    strTitle = m_strTitle;
    lstUrl = m_lstPictureUrl;
    m_eCurrentType = TYPE_DEFAULT;
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
        if(m_eCurrentType == TYPE_DEFAULT)
        {
            //do nothing
/*			QByteArray allBytes;
			while(1)
			{
				QByteArray bytes = reply->readAll();
				qDebug() << QString::fromUtf8(bytes);
				if (bytes.count() <= 0)
					break;
				allBytes.append(bytes);
			}
			QString str = QString::fromUtf8(allBytes).replace("\r", "").replace("\n", "").replace("\t", "");;
			qDebug() << str;*/
        }
        else if(m_eCurrentType == TYPE_ALL_PIC_URL)
        {
			QByteArray bytes = reply->readAll();
			QString str = QString::fromUtf8(bytes);
			str = str.replace("\r", "").replace("\n", "").replace("\t", "");
			qDebug() << str;

			QString tag1 = "<title>", tag2 = "</title>";
			if (str.contains(tag1) && str.contains(tag2))
			{
				int nBegin = str.indexOf(tag1);
				int nEnd = str.indexOf(tag2);
				m_strTitle = str.mid(nBegin + tag1.count(), nEnd - (nBegin + tag1.count())).trimmed();
			}

			while(1)
			{
				QString flag = "<img src=";
				int i = str.indexOf(flag);
				if (i < 0)
					break;
				str = str.mid(i + flag.count() + 1);
				int j = str.indexOf("\"");
				QString url = str.left(j);
				if(url.endsWith(".jpg"))
					m_lstPictureUrl << url;
				str = str.mid(j);
			}
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
