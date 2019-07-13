#include "localhttpclientapi.h"

LocalHTTPClientAPI::LocalHTTPClientAPI(const QString &serverHostUrl, const QString &code):
    rootDomain(serverHostUrl),userCode(code)
{
    nAccessManager = new QNetworkAccessManager();
    connect(nAccessManager,&QNetworkAccessManager::sslErrors,this,&LocalHTTPClientAPI::handleSslErrors);

    qRegisterMetaType<HTTPObject>("HTTPObjectReply");
}

void LocalHTTPClientAPI::printSslInformation()
{
    QString supportsSsl = QSslSocket::supportsSsl() ? "Yes" : "No";
    print( "Supports ssl: "+ supportsSsl + "\n" + "Build version: " + QSslSocket::sslLibraryBuildVersionString()
           + "\nRuntime version: " + QSslSocket::sslLibraryVersionString());
}

void LocalHTTPClientAPI::sendGetRequest(const QString &method, const QString &urlParameter)
{
    if(!_isBusy)
        _isBusy = true;
    else
    {
        emit sendErrorString("Client is busy. Awaiting response from server.");
        return;
    }

    QString rDomain;
    if(rootDomain != QString())
        rDomain = rootDomain;
    else
        return;

    if(rDomain.at(rDomain.length() - 1) != '/')
        rDomain.append('/');

    QString codePart;

    if(userCode != QString())
        codePart = "?code=" + userCode;

    if(urlParameter != QString() && userCode == QString())
        codePart += "?";
    else if(urlParameter != QString())
        codePart += "&";


    QString fullServerUrl = rootDomain + method + codePart + urlParameter;

    print(fullServerUrl);

    tempReply = nAccessManager->get(QNetworkRequest(QUrl(fullServerUrl)));

    connect(tempReply,&QNetworkReply::finished,this,&LocalHTTPClientAPI::handleReply);
    connect(tempReply,&QNetworkReply::finished,tempReply,&QNetworkReply::deleteLater);
}

void LocalHTTPClientAPI::sendPostRequest(const QString &method, const QJsonObject JSON)
{
    if(!_isBusy)
        _isBusy = true;
    else
    {
        emit sendErrorString("Client is busy. Awaiting response from server.");
        return;
    }

    QString rDomain;
    if(rootDomain != QString())
        rDomain = rootDomain;
    else
        return;

    if(rDomain.at(rDomain.length() - 1) != '/')
        rDomain.append('/');

    QString fullServerUrl = rootDomain + method + "?code=" + userCode;

    print(fullServerUrl);
    QUrl serverUrl = QUrl(fullServerUrl);
    QNetworkRequest req(serverUrl);

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    tempReply = nAccessManager->post(req,QJsonDocument(JSON).toJson());

    connect(tempReply,&QNetworkReply::finished,this,&LocalHTTPClientAPI::handleReply);
    connect(tempReply,&QNetworkReply::finished,tempReply,&QNetworkReply::deleteLater);
}

void LocalHTTPClientAPI::handleReply()
{
    QByteArray data = tempReply->readAll();
    HTTPObject object(data);

    if(tempReply->error())
    {
        print("(Failed) Data:" + tempReply->errorString());
        emit sendErrorString(tempReply->errorString());
    }
    else
    {
        print("(Succes) Data:" + QString::fromStdString(data.toStdString()));
        emit sendHTTPObject(object);
    }

    _isBusy = false;
}

void LocalHTTPClientAPI::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    Q_UNUSED(reply);

    print("Errors:");

    for (QSslError error : errors)
        addEntry(error.errorString());

    extractEntries();
}

void LocalHTTPClientAPI::handleRedirection(const QUrl &url)
{
    QString string = url.toString();
    print("Redirection url: " + string);
}

bool LocalHTTPClientAPI::isBusy() const
{
    return _isBusy;
}

QString LocalHTTPClientAPI::getUserCode() const
{
    return userCode;
}

void LocalHTTPClientAPI::setUserCode(const QString &value)
{
    userCode = value;
}

QString LocalHTTPClientAPI::getRootDomain() const
{
    return rootDomain;
}
