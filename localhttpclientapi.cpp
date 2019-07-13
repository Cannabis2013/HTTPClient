#include "localhttpclientapi.h"

LocalHTTPClientAPI::LocalHTTPClientAPI(const QString &serverHostUrl, const QString &code):
    rootDomain(serverHostUrl),userCode(code)
{
    qNAM = new QNetworkAccessManager();
    connect(qNAM,&QNetworkAccessManager::sslErrors,this,&LocalHTTPClientAPI::handleSslErrors);

    qRegisterMetaType<HTTPObject>("HTTPObject");

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
        throw overflow_error("Client is busy and awaiting response from an unresolved request");
    }

    QString fullServerUrl;

    try {
        processAndAssembleUrl(fullServerUrl,method,urlParameter);
    } catch (invalid_argument e) {
        print(e.what());
        return;
    }

    print(fullServerUrl);

    tempReply = qNAM->get(QNetworkRequest(QUrl(fullServerUrl)));

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
    tempReply = qNAM->post(req,QJsonDocument(JSON).toJson());

    connect(tempReply,&QNetworkReply::finished,this,&LocalHTTPClientAPI::handleReply);
    connect(tempReply,&QNetworkReply::finished,tempReply,&QNetworkReply::deleteLater);
}

void LocalHTTPClientAPI::sendDeleteRequest(const QString &method, const QString &urlParameter)
{
    if(!_isBusy)
        _isBusy = true;
    else
    {
        emit sendErrorString("Client is busy. Awaiting response from server.");
        return;
    }

    QString fullServerUrl;
    try {
        processAndAssembleUrl(fullServerUrl,method,urlParameter);
    } catch (invalid_argument e) {
        print(e.what());
        return;
    }

    print(fullServerUrl);

    tempReply = qNAM->deleteResource(QNetworkRequest(QUrl(fullServerUrl)));

    connect(tempReply,&QNetworkReply::finished,this,&LocalHTTPClientAPI::handleReply);
    connect(tempReply,&QNetworkReply::finished,tempReply,&QNetworkReply::deleteLater);
}

void LocalHTTPClientAPI::handleReply()
{
    QByteArray data = tempReply->readAll();
    HTTPObject object(data);

    if(!tempReply->error())
    {
        print("(Succes) Data:" + QString::fromStdString(data.toStdString()));
        emit sendHTTPObject(object);
    }
    else
    {
        print("(Failed) Data:" + tempReply->errorString());
        emit sendErrorString(tempReply->errorString());
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

void LocalHTTPClientAPI::processAndAssembleUrl(QString &hostUrl, QString methodName, QString urlParameter)
{
    if(rootDomain != QString())
        hostUrl = rootDomain;
    else
        throw invalid_argument("Host url not valid or not set");

    if(hostUrl.at(hostUrl.length() - 1) != '/')
        hostUrl.append('/');

    if(methodName.at(methodName.length() - 1) != '/')
        methodName.append('/');

    methodName += urlParameter;

    hostUrl += methodName;

    if(userCode != QString())
        hostUrl += "?code=" + userCode;
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
