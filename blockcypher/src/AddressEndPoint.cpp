/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 10 2015
 */

#include <blockcypher/AddressEndPoint.hpp>

#include <QJsonObject>

namespace blockcypher {
namespace AddressEndPoint {

Reply::Reply(QNetworkReply * reply, const QString & walletName, bool unspentOnly, uint limit, uint confirmations)
    : blockcypher::Reply(reply)
    , _walletName(walletName)
    , _unspentOnly(unspentOnly)
    , _limit(limit)
    , _confirmations(confirmations)
    , _response(BlockCypherResponse::Pending) {
}

void Reply::processReply() {

    if(_reply->bytesAvailable() == 0) {
        _response = BlockCypherResponse::catch_all;
    } else {

        //qDebug() << "URL:" << _reply->request().url().toString();
        //qDebug() << "RESPONSE:" << _rawResponse;

        if(_error == QNetworkReply::NoError) {

            _response = BlockCypherResponse::Fetched;

            for(QJsonObject & obj : BlockCypher::rawToQJsonObjects(_rawResponse)) {
                _addresses.push_back(Address(obj));
            }

        } else if(_reply->error() == QNetworkReply::NetworkError::ContentNotFoundError) {
            _response = BlockCypherResponse::WalletDoesNotExist;
            qDebug() << "Wallet/Address does not exist error.";
        } else  {

            _response = BlockCypherResponse::catch_all;
            qDebug() << "QNetworkReplyFinished error.";
        }
    }

    emit done(_response);
}

const char * Reply::errorMessage() const {

    if(_error != QNetworkReply::NoError) {

        switch(_response) {

            case BlockCypherResponse::WalletDoesNotExist: return "Wallet does not exist";
            case BlockCypherResponse::catch_all: return "Catch all error";

            default:
                Q_ASSERT(false);
        }

    } else
        return "";
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

std::vector<Address> Reply::addresses() const {
    return _addresses;
}

}
}
