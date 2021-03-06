/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 9 2015
 */

#include <blockcypher/GetWallet.hpp>

#include <QJsonObject>

namespace blockcypher {
namespace GetWallet {

Reply::Reply(QNetworkReply * reply, const QString & name)
    : blockcypher::Reply(reply)
    , _name(name)
    , _response(BlockCypherResponse::Pending) {
}

const char * Reply::errorMessage() const {

    if(_error != QNetworkReply::NoError) {

        switch(_response) {

            case BlockCypherResponse::DoesNotExist: return "Wallet does not exist";
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

Wallet Reply::wallet() const {
    return _wallet;
}

void Reply::processReply() {

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

            _response = BlockCypherResponse::Returned;
            _wallet = Wallet(blockcypher::rawToQJsonObject(response));
            Q_ASSERT(_wallet._name == _name);
            qDebug() << "GetWallet: Wallet found.";

        } else if(_reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
            _response = BlockCypherResponse::DoesNotExist;
            qDebug() << "GetWallet: Wallet doesnt exist.";
        } else {

            _response = BlockCypherResponse::catch_all;
            qDebug() << "GetWallet: QNetworkReplyFinished error: " << QString(response);
        }
    }

    emit done(_response);
}

}
}
