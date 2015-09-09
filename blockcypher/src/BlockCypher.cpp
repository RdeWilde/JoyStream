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

namespace BlockCypher {

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

QJsonObject rawToQJsonObject(const QByteArray & data) {

    // Parse into json
    QJsonDocument jsonResponse = QJsonDocument::fromJson(data);

    // Turn into json and return
    return jsonResponse.object();
}

}
