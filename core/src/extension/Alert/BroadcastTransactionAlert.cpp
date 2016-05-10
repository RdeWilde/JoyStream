/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/BroadcastTransactionAlert.hpp>

BroadcastTransactionAlert::BroadcastTransactionAlert(const libtorrent::sha1_hash &infoHash,
                                                     const Coin::Transaction & tx)
    : TorrentPluginAlert(infoHash),
      _transaction(tx) {
}

BroadcastTransactionAlert::BroadcastTransactionAlert(const BroadcastTransactionAlert & alert)
    : TorrentPluginAlert(alert),
      _transaction(alert.transaction()) {
}

int BroadcastTransactionAlert::type() const {
    return alert_type;
}

char const* BroadcastTransactionAlert::what() const {
    return "BroadcastTransactionAlert";
}

std::string BroadcastTransactionAlert::message() const {
    return std::string("BroadcastTransactionAlert::message: IMPLEMENT LATER");
}

int BroadcastTransactionAlert::category() const {
    return libtorrent::alert::stats_notification; //shouldn't this be status_notification instead of stats_notification?
}

std::auto_ptr<libtorrent::alert> BroadcastTransactionAlert::clone() const {
    return std::auto_ptr<alert>(new BroadcastTransactionAlert(*this));
}

Coin::Transaction BroadcastTransactionAlert::transaction() const {
    return _transaction;
}

void BroadcastTransactionAlert::setTransaction(const Coin::Transaction &tx) {
    _transaction = tx;
}
