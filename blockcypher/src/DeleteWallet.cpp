/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2015
 */

#include <blockcypher/DeleteWallet.hpp>

namespace BlockCypher {
namespace DeleteWallet {


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

void Reply::QNetworkReplyFinished() {

    if(_reply->bytesAvailable() == 0) {
        //_response = BlockCypherResponse::UnknownError;
        _response = BlockCypherResponse::Deleted;
    } else {

        // Get response data, without emptying QIODevice
        //(readAll consumes devices, which breaks multiple parsing calls,
        // e.g. due to finished signal and explicit parse call.)
        QByteArray response = _reply->peek(_reply->bytesAvailable());

        // If there was an error, throw exception
        QNetworkReply::NetworkError e = _reply->error();

        if(e == QNetworkReply::NoError) {
            _response = BlockCypherResponse::Deleted;
            //} else if(e == QNetworkReply::ContentConflictError)
            //_response = BlockCypherResponse::AlreadyExists;
        } else if(e == QNetworkReply::ContentNotFoundError) {
            _response = BlockCypherResponse::DidntExist;
            //throw std::runtime_error("Wallet does not exist");
        } else {

            //_response = BlockCypherResponse::catch_all;
            qDebug() << QString(response);
        }
    }

    emit done(_response);
}

}
}
