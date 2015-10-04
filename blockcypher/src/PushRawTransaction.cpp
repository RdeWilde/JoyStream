/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 11 2015
 */

#include <blockcypher/PushRawTransaction.hpp>

#include <QJsonDocument>
#include <QJsonObject>

namespace BlockCypher {
namespace PushRawTransaction {

Reply::Reply(QNetworkReply * reply, const Coin::Transaction & toBeBroadcasted)
    : BlockCypher::Reply(reply)
    , _toBeBroadcasted(toBeBroadcasted)
    , _response(BlockCypherResponse::Pending) {
}

const char * Reply::errorMessage() const {

    if(_error != QNetworkReply::NoError) {

        switch(_response) {

            case BlockCypherResponse::InvalidTx: return "Invalid transaction";
            case BlockCypherResponse::catch_all: return "Catch all error";

            default:
                Q_ASSERT(false);
        }

    } else
        return "";
}

Coin::Transaction Reply::toBeBroadcasted() const {
    return _toBeBroadcasted;
}

BlockCypherResponse Reply::response() const {
    return _response;
}

TX Reply::returned() const {
    return _returned;
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

            //std::string str = response.toStdString();
            //_returned = TX(str);

            _response = BlockCypherResponse::Broadcasted;

        } else if(e == QNetworkReply::ProtocolInvalidOperationError) {

            /**
            QJsonDocument doc = QJsonDocument::fromJson(response);
            QJsonObject result = doc.object();

            _response = BlockCypherResponse::InvalidTx;
            */

            qDebug() << "ProtocolInvalidOperationError when broadcasting: " << response;

            _response = BlockCypherResponse::Broadcasted;

        } else {

            _response = BlockCypherResponse::catch_all;
            qDebug() << "QNetworkReplyFinished error: " << QString(response);
        }
    }

    emit done(_response);
}

}
}
