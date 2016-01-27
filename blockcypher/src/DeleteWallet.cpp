/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2015
 */

#include <blockcypher/DeleteWallet.hpp>

namespace blockcypher {
namespace DeleteWallet {


Reply::Reply(QNetworkReply * reply, const QString & name)
    : blockcypher::Reply(reply)
    , _name(name)
    , _response(BlockCypherResponse::Pending) {
}

const char * Reply::errorMessage() const {

    if(_error != QNetworkReply::NoError) {

        switch(_response) {

            case BlockCypherResponse::DidntExist: return "Wallet didn't' exist";
            case BlockCypherResponse::InvalidName: return "Invalid wallet name";
            case BlockCypherResponse::catch_all: return "Catch all error";

            default:
                Q_ASSERT(false);
        }

    } else
        return "";
}

QString Reply::name() const {
    return _name;
}

BlockCypherResponse Reply::response() const {
    return _response;
}

void Reply::processReply() {

    if(_reply->bytesAvailable() == 0) {
        //_response = BlockCypherResponse::UnknownError;
        Q_ASSERT(_error == QNetworkReply::NoError);
        _response = BlockCypherResponse::Deleted;
    } else {

        if(_error == QNetworkReply::NoError) {
            _response = BlockCypherResponse::Deleted;
            qDebug() << "DeleteWallet: Wallet deleted.";
        } else if(_error == QNetworkReply::ContentNotFoundError) {
            _response = BlockCypherResponse::DidntExist;
            qDebug() << "DeleteWallet: Wallet doesnt exist.";
        } else {
            _response = BlockCypherResponse::catch_all;
            qDebug() << "DeleteWallet: QNetworkReplyFinished error.";
        }
    }

    emit done(_response);
}

}
}
