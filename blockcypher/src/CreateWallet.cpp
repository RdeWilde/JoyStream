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

Wallet Reply::response() const {
    return _response;
}

void Reply::QNetworkReplyError(QNetworkReply::NetworkError code) {
    emit error(code);
}

void Reply::QNetworkReplyFinished() {

    // Convert data from
    _response = Wallet(replyToQJsonObject(_reply));

    // Send signal about wallet being created
    emit walletCreated(_response);
}


}
}
