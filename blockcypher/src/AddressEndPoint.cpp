/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/AddressEndPoint.hpp>

#include <QJsonObject>

namespace BlockCypher {
namespace AddressEndPoint {

Reply::Reply(QNetworkReply * reply, const QString & walletName, bool unspentOnly, uint limit, uint confirmations)
    : BlockCypher::Reply(reply)
    , _walletName(walletName)
    , _unspentOnly(unspentOnly)
    , _limit(limit)
    , _confirmations(confirmations)
    , _response(BlockCypherResponse::Pending) {
}

void Reply::QNetworkReplyFinished() {

    if(_reply->bytesAvailable() == 0) {
        _response = BlockCypherResponse::catch_all;
    } else {

        // Get response data, without emptying QIODevice
        //(readAll consumes devices, which breaks multiple parsing calls,
        // e.g. due to finished signal and explicit parse call.)
        QByteArray response = _reply->peek(_reply->bytesAvailable());

        //qDebug() << "URL:" << _reply->request().url().toString();
        //qDebug() << "RESPONSE:" << response;

        // If there was an error, throw exception
        QNetworkReply::NetworkError e = _reply->error();

        if(e == QNetworkReply::NoError) {

            _response = BlockCypherResponse::Fetched;
            _address = Address(BlockCypher::rawToQJsonObject(response));

        } else if(_reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
            _response = BlockCypherResponse::WalletDoesNotExist;
        } else  {

            _response = BlockCypherResponse::catch_all;
            //qDebug() << QString(response);
        }
    }

    emit done(_response);
}

QString Reply::walletName() const {
    return _walletName;
}

bool Reply::unspentOnly() const {
    return _unspentOnly;
}

uint Reply::limit() const {
    return _limit;
}

uint Reply::confirmations() const {
    return _confirmations;
}

BlockCypherResponse Reply::response() const {
    return _response;
}

Address Reply::address() const {
    return _address;
}

}
}
