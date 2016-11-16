/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

#ifndef JOYSTREAM_APPKIT_TRANSACTION_SEND_BUFFER_HPP
#define JOYSTREAM_APPKIT_TRANSACTION_SEND_BUFFER_HPP

#include <bitcoin/SPVWallet.hpp>

namespace joystream {
namespace appkit {

class TransactionSendBuffer
{

public:
    TransactionSendBuffer(bitcoin::SPVWallet*);

    size_t size() const;

    void flush();

    void insert(const Coin::Transaction &);

private:
    bitcoin::SPVWallet* _wallet;
    std::map<Coin::TransactionId, Coin::Transaction> _transactions;
};

}
}
#endif // JOYSTREAM_APPKIT_TRANSACTION_SEND_BUFFER_HPP
