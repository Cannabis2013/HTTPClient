#ifndef HTTPOBJECTREPLY_H
#define HTTPOBJECTREPLY_H

#include <QJsonDocument>
#include <qjsonobject.h>
#include <qjsonarray.h>


class HTTPObject
{
public:
    HTTPObject();
    HTTPObject(const QByteArray &data);

    enum objectType {Object,Array};

    objectType getObjectType() const;

    QJsonObject getObject() const;
    QJsonArray getArray() const;

    QByteArray getData() const;

private:
    QJsonDocument _jSONDocument;
    QByteArray _data;
};

#endif // HTTPOBJECTREPLY_H
