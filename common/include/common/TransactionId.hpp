#ifndef TRANSACTION_ID_HPP
#define TRANSACTION_ID_HPP

#include <common/FixedUCharArray.hpp>

#define TXID_BYTE_LENGTH 20

namespace Coin {

// Define Bitcoin types as spesific length fixed uchar arrays
typedef FixedUCharArray<TXID_BYTE_LENGTH> TransactionId;

}

#endif // TRANSACTION_ID_HPP
