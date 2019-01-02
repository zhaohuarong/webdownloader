#ifndef ACCESSMANAGER_H
#define ACCESSMANAGER_H

#include <QObject>

#define g_pAccessManager AccessManager::instance()

class QNetworkReply;
class QNetworkAccessManager;

class AccessManager : public QObject
{
    Q_OBJECT
public:
    static AccessManager *instance();
    void getHTML(const QString &url);

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
};

#endif // ACCESSMANAGER_H
