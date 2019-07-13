#include <QApplication>

#include "localhttpclientapi.h"
#include <QJsonObject>

QT_REQUIRE_CONFIG(ssl);

/*
 * Code to API CALL: "wQ71HOHNXNaHkDxnnaZ7kj6ujWYW2xBsV1VXxRqp8C/83IgryF1ADA=="
 */
#define myCode "wQ71HOHNXNaHkDxnnaZ7kj6ujWYW2xBsV1VXxRqp8C/83IgryF1ADA==";


int main(int argc, char *argv[])
{
    QString f = myCode;
    QApplication a(argc, argv);

    QString code = myCode;
    LocalHTTPClientAPI httpClient("https://dartservice.azurewebsites.net/api/",code);
    httpClient.printSslInformation();
    if(!QSslSocket::supportsSsl())
        return -1;

    httpClient.sendGetRequest("GetTournaments");

    return a.exec();
}
