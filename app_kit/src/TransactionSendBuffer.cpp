/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#include <app_kit/TransactionSendBuffer.hpp>

namespace joystream {
namespace appkit {

TransactionSendBuffer::TransactionSendBuffer(bitcoin::SPVWallet *wallet)
    : _wallet(wallet) {
}

size_t TransactionSendBuffer::size() const {
    return _transactions.size();
}

void TransactionSendBuffer::flush() {
    if(size() == 0)
        return;

    if(_wallet->isConnected()) {

        for(auto &entry : _transactions)
        {
            std::cout << "Broadcasting Transaction: " << entry.first.toRPCByteOrder() << std::endl;
            _wallet->broadcastTx(entry.second);
            // add to pending queue waiting for notification that it was propagated (txUpdated signal) or rejected (txRejected) signal
                // remove when one of those signals is received
            // if spvwallet looses connection return txs remaining in this queue to the send queue
            // to retry transmission (because netsync will clear its send queue)
        }

        _transactions.clear();
    }
}

void TransactionSendBuffer::insert(const Coin::Transaction &tx) {
    _transactions[Coin::TransactionId::fromTx(tx)] = tx;
    flush();
}

}
}
