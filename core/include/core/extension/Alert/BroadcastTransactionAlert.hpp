/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BROADCAST_TRANSACTION_ALERT_HPP
#define BROADCAST_TRANSACTION_ALERT_HPP

#include <core/extension/Alert/TorrentPluginAlert.hpp>
#include <core/extension/Alert/AlertTypes.hpp>
#include <CoinCore/CoinNodeData.h>

class BroadcastTransactionAlert : public TorrentPluginAlert
{
public:

    // Public member required for alert_cast
    const static int alert_type = BROADCAST_TRANSACTION_ALERT_ID;

    BroadcastTransactionAlert(const libtorrent::sha1_hash & infoHash,
                              const Coin::Transaction & tx);

    // Copy constructor
    BroadcastTransactionAlert(const BroadcastTransactionAlert & alert);

    // Virtual routines from libtorrent::alert
    virtual int type() const;
    virtual char const* what() const;
    virtual std::string message() const;
    virtual int category() const;
    virtual std::auto_ptr<libtorrent::alert> clone() const;

    // Getters and setters
    Coin::Transaction transaction() const;
    void setTransaction(const Coin::Transaction &tx);

private:

    Coin::Transaction _transaction;
};

#endif // BROADCAST_TRANSACTION_ALERT_HPP
