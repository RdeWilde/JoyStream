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
    , _requested(requested)
    , _response(BlockCypherResponse::Pending) {
}

const char * Reply::errorMessage() const {

    if(_error != QNetworkReply::NoError) {

        switch(_response) {

            case BlockCypherResponse::AlreadyExists: return "Wallet already exists";
            case BlockCypherResponse::InvalidName: return "Invalid wallet name";
            case BlockCypherResponse::catch_all: return "Catch all error";

            default:
                Q_ASSERT(false);
        }

    } else
        return "";
}

void Reply::processReply() {

    if(_reply->bytesAvailable() == 0) {
        _response = BlockCypherResponse::catch_all;
    } else {

        if(_error == QNetworkReply::NoError) {

            _response = BlockCypherResponse::Created;
            _created = Wallet(BlockCypher::rawToQJsonObject(_rawResponse));
            //qDebug() << "Wallet created";, not always create, get uses same I think

        } else if(_error == QNetworkReply::ContentConflictError) {
            _response = BlockCypherResponse::AlreadyExists;
            qDebug() << "Wallet already exists.";
        } else {

            _response = BlockCypherResponse::catch_all;
            qDebug() << "QNetworkReplyFinished error.";
        }
    }

    emit done(_response);
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

}
}
