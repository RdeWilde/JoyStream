/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UTILITIES_HPP
#define COIN_UTILITIES_HPP

#include <common/Base58CheckEncodable.hpp> // version macroes

class QByteArray;
class uchar_vector;

namespace Coin {

QByteArray toByteArray(const uchar_vector & raw);

// Used to provide version array for mSIGNA libs.
// Returns 1 byte version byte for p2pkh and p2sh addresses on given network
enum class Network;

const unsigned char testnet3AddressVersions[] = {P2PKH_ADDRESS_TESTNET3_VERSION_VALUE, P2SH_ADDRESS_TESTNET3_VERSION_VALUE};
const unsigned char mainnetAddressVersions[] = {P2PKH_ADDRESS_MAINNET_VERSION_VALUE, P2SH_ADDRESS_MAINNET_VERSION_VALUE};

const unsigned char * networkToAddressVersions(Network network);

}
#endif // COIN_UTILITIES_HPP
