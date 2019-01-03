#ifndef ACCESSMANAGER_H
#define ACCESSMANAGER_H

#include <QObject>

#define g_pAccessManager AccessManager::instance()

class QNetworkReply;
class QNetworkAccessManager;

enum E_ACCESS_TYPE
{
    TYPE_DEFAULT = 0,
    TYPE_ALL_PIC_URL
};

class AccessManager : public QObject
{
    Q_OBJECT
public:
    static AccessManager *instance();
    void getHTML(const QString &url);
    void getTitleAndAllPictureUrl(const QString &url, QString &strTitle, QStringList &lstUrl);

signals:
    void sigFinishedSlot();

private:
    explicit AccessManager(QObject *parent = nullptr);
    void sync();

private slots:
    void finishedSlot(QNetworkReply *reply);

private:
    static AccessManager *m_pAccessManager;
    QNetworkAccessManager *m_accessManager;
    QString m_strTitle;
    QStringList m_lstPictureUrl;
    E_ACCESS_TYPE m_eCurrentType;
};

#endif // ACCESSMANAGER_H
