/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 8 2015
 */

#include <blockcypher/CreateWallet.hpp>

#include <QJsonObject>

namespace BlockCypher {
namespace CreateWallet {

Reply::Reply(QNetworkReply * reply, const Wallet & requested)
    : BlockCypher::Reply(reply)
    , _requested(requested) {
}

Wallet Reply::requested() const {
    return _requested;
}

BlockCypherResponse Reply::response() const {
    return _response;
}

Wallet Reply::created() const {
    return _created;
}

void Reply::QNetworkReplyFinished() {

    if(_reply->bytesAvailable() == 0) {
        _response = BlockCypherResponse::catch_all;
    } else {

        // Get response data, without emptying QIODevice
        //(readAll consumes devices, which breaks multiple parsing calls,
        // e.g. due to finished signal and explicit parse call.)
        QByteArray response = _reply->peek(_reply->bytesAvailable());

        // If there was an error, throw exception
        QNetworkReply::NetworkError e = _reply->error();

        if(e == QNetworkReply::NoError) {

            _response = BlockCypherResponse::Created;
            _created = Wallet(BlockCypher::rawToQJsonObject(response));

        } else if(e == QNetworkReply::ContentConflictError)
            _response = BlockCypherResponse::AlreadyExists;
        else {

            _response = BlockCypherResponse::catch_all;
            qDebug() << QString(response);
        }
    }

    emit done(_response);
}

}
}
