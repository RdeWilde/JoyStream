/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 11 2015
 */

#include <blockcypher/FundWalletFromFaucet.hpp>

#include <QJsonDocument>
#include <QJsonObject>

namespace BlockCypher {
namespace FundWalletFromFaucet {

Reply::Reply(QNetworkReply * reply)
    : BlockCypher::Reply(reply)
    , _response(BlockCypherResponse::Pending) {
}

const char * Reply::errorMessage() const {

    if(_error != QNetworkReply::NoError) {

        switch(_response) {
            case BlockCypherResponse::Error: return "Protocol Error";
            case BlockCypherResponse::catch_all: return "Catch all error";

            default:
                Q_ASSERT(false);
        }

    } else
        return "";
}

BlockCypherResponse Reply::response() const {
    return _response;
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

            _response = BlockCypherResponse::Funded;

        } else if(e == QNetworkReply::ProtocolInvalidOperationError) {

            _response = BlockCypherResponse::Error;

        } else {

            _response = BlockCypherResponse::catch_all;
            qDebug() << "QNetworkReplyFinished error: " << QString(response);
        }
    }

    emit done(_response);
}

}
}
