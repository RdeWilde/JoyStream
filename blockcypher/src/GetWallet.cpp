/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 9 2015
 */

#include <blockcypher/GetWallet.hpp>

#include <QJsonObject>

namespace BlockCypher {
namespace GetWallet {

Reply::Reply(QNetworkReply * reply, const QString & name)
    : BlockCypher::Reply(reply)
    , _name(name)
    , _response(BlockCypherResponse::Pending) {
}

QString Reply::name() const {
    return _name;
}

BlockCypherResponse Reply::response() const {
    return _response;
}

Wallet Reply::wallet() const {
    return _wallet;
}

void Reply::QNetworkReplyFinished() {

    if(_reply->bytesAvailable() == 0) {
        _response = BlockCypherResponse::None;
    } else {

        // Get response data, without emptying QIODevice
        //(readAll consumes devices, which breaks multiple parsing calls,
        // e.g. due to finished signal and explicit parse call.)
        QByteArray response = _reply->peek(_reply->bytesAvailable());

        // If there was an error, throw exception
        QNetworkReply::NetworkError e = _reply->error();

        if(e == QNetworkReply::NoError) {

            _response = BlockCypherResponse::Returned;
            _wallet = Wallet(BlockCypher::rawToQJsonObject(response));
            Q_ASSERT(_wallet._name == _name);

        } else if(_reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
            _response = BlockCypherResponse::DoesNotExist;
        } else {

            _response = BlockCypherResponse::None;
            qDebug() << QString(response);
        }
    }

    emit done(_response);
}

}
}
