/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <blockcypher/BlockCypher.hpp>

#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>

namespace blockcypher {

Reply::Reply(QNetworkReply * reply)
    : _reply(reply) {

    /**
    QObject::connect(reply,
                     SIGNAL(error(QNetworkReply::NetworkError)),
                     this,
                     SLOT(QNetworkReplyError(QNetworkReply::NetworkError)));
    */

    QObject::connect(reply,
                     &QNetworkReply::finished,
                     this,
                     &Reply::QNetworkReplyFinished);
}

Reply::~Reply() {
    // http://stackoverflow.com/questions/10346742/qt-shall-we-delete-qnetworkreply-reply-received-on-qnetworkaccessmanager-signal;
    _reply->deleteLater();
}

QNetworkReply * Reply::reply() const {
    return _reply;
}

QByteArray Reply::rawResponse() const {
    return _rawResponse;
}

QNetworkReply::NetworkError Reply::error() const {
    return _error;
}

QString Reply::errorReport() const {
    return QString(errorMessage()) + ": " + _rawResponse;
}

void Reply::QNetworkReplyFinished() {

    // Get response data, without emptying QIODevice
    //(readAll consumes devices, which breaks multiple parsing calls,
    // e.g. due to finished signal and explicit parse call.)
    _rawResponse = _reply->peek(_reply->bytesAvailable());

    _error = _reply->error();

    processReply();
}

/**
void Reply::QNetworkReplyError(QNetworkReply::NetworkError code) {
    emit error(code);
}
*/

/**
QJsonObject replyToQJsonObject(QNetworkReply * reply) {

    // Get response data, without emptying QIODevice
    //(readAll consumes devices, which breaks multiple parsing calls,
    // e.g. due to finished signal and explicit parse call.)
    QByteArray response = reply->peek(reply->bytesAvailable());

    // If there was an error, throw exception
    QNetworkReply::NetworkError e = reply->error();

    if(e != QNetworkReply::NoError) {

        qDebug() << QString(response);

        throw std::runtime_error(response.toStdString());
    }

    // Parse into json
    QJsonDocument jsonResponse = QJsonDocument::fromJson(response);

    // Turn into json and return
    return jsonResponse.object();
}
*/

// Use this method when only a single object is excpected as a response
QJsonObject rawToQJsonObject(const QByteArray & data) {

    // Parse into json
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data);

    if(!jsonResponse.isObject()) {
        throw std::runtime_error("response is not an object");
    }

    return jsonResponse.object();
}

// Use this method when expecting an array of objects as a response
std::vector<QJsonObject> rawToQJsonObjects(const QByteArray & data) {

    // Parse into json
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data);

    // Response maybe a single object or an array of objects
    std::vector<QJsonObject> objects;

    if(jsonResponse.isArray()){
        for(const QJsonValue & value : jsonResponse.array()){
            objects.push_back(value.toObject());
        }
    } else {
        objects.push_back(jsonResponse.object());
    }

    return objects;
}

}
