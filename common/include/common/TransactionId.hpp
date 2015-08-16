#ifndef COIN_TRANSACTION_ID_HPP
#define COIN_TRANSACTION_ID_HPP

#include <common/UCharArray.hpp>

#define TXID_BYTE_LENGTH 20

namespace Coin {

// Define Bitcoin types as spesific length fixed uchar arrays
typedef UCharArray<TXID_BYTE_LENGTH> TransactionId;

}

#endif // COIN_TRANSACTION_ID_HPP
