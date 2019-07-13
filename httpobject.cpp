#include "httpobject.h"

HTTPObject::HTTPObject()
{
}

HTTPObject::HTTPObject(const QByteArray &data)
{
    _data = data;
    _jSONDocument = QJsonDocument::fromBinaryData(data);
}

HTTPObject::objectType HTTPObject::getObjectType() const
{
    if(_jSONDocument.isArray())
        return objectType::Array;
    else
        return objectType::Object;
}

QJsonObject HTTPObject::getObject() const
{
    if(!_jSONDocument.isArray())
        return _jSONDocument.object();
    else
        return QJsonObject();
}

QJsonArray HTTPObject::getArray() const
{
    if(_jSONDocument.isArray())
        return _jSONDocument.array();
    else
        return QJsonArray();
}

QByteArray HTTPObject::getData() const
{
    return _data;
}
