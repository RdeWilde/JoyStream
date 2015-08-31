#ifndef COIN_TRANSACTION_ID_HPP
#define COIN_TRANSACTION_ID_HPP

#include <common/UCharArray.hpp>

#define TXID_BYTE_LENGTH 32

namespace Coin {

// Define Bitcoin types as spesific length fixed uchar arrays
typedef UCharArray<TXID_BYTE_LENGTH> TransactionId;

}

Q_DECLARE_METATYPE(Coin::TransactionId)

#endif // COIN_TRANSACTION_ID_HPP
