#ifndef HTTPINTERFACE_H
#define HTTPINTERFACE_H

#include <qnetworkaccessmanager.h>
#include <qnetworkrequest.h>
#include <QNetworkReply>
#include <qnetworkconfiguration.h>
#include <qlist.h>

#include "httpobject.h"
#include "myobject.h"

class LocalHTTPClientAPI : public MyObject
{
    Q_OBJECT
public:
    LocalHTTPClientAPI(const QString &serverHostUrl, const QString &code = QString());

    void printSslInformation();
    void sendGetRequest(const QString &method,
                        const QString &urlParameter = QString());
    void sendPostRequest(const QString &method,
                          const QJsonObject JSON);

    QString getRootDomain() const;

    QString getUserCode() const;
    void setUserCode(const QString &value);

    bool isBusy() const;

signals:
    void sendHTTPObject(const HTTPObject &obj);
    void sendErrorString(const QString &err);
    void sendSslErrorStrings(const QStringList &err);

private slots:

    void handleReply();
    void handleSslErrors(QNetworkReply *reply,const QList<QSslError>&errors);
    void handleRedirection(const QUrl &url);
private:

    QNetworkAccessManager* nAccessManager;
    QNetworkReply* tempReply;
    QString rootDomain;
    QString userCode;
    bool _isBusy = false;
};

#endif // HTTPINTERFACE_H
