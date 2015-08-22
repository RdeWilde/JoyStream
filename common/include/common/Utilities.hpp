/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef COIN_UTILITIES_HPP
#define COIN_UTILITIES_HPP

#include <common/Base58CheckEncodable.hpp> // version macroes
#include <common/AddressType.hpp>

class QByteArray;
class uchar_vector;

namespace Coin {

QByteArray toByteArray(const uchar_vector & vector);

uchar_vector toUCharVector(const QByteArray & array);

// Used to provide version array for mSIGNA libs.
// Returns 1 byte version byte for p2pkh and p2sh addresses on given network
enum class Network;

const unsigned char testnet3AddressVersions[] = {P2PKH_ADDRESS_TESTNET3_VERSION_VALUE, P2SH_ADDRESS_TESTNET3_VERSION_VALUE};
const unsigned char mainnetAddressVersions[] = {P2PKH_ADDRESS_MAINNET_VERSION_VALUE, P2SH_ADDRESS_MAINNET_VERSION_VALUE};

const unsigned char * networkToAddressVersions(Network network);

/**
 * Routines for generating version bytes for bip32 extended key serialization
 * 4 byte: version bytes (mainnet: 0x0488B21E public, 0x0488ADE4 private; testnet: 0x043587CF public, 0x04358394 private)
 */
unsigned int extendedPrivateKeyVersionBytes(Network network);
unsigned int extendedPublicKeyVersionBytes(Network network);

/**
 * Process Bitcoin addresses according to
 * https://en.bitcoin.it/wiki/List_of_address_prefixes
 */

// Generate version byte
unsigned int toBase58CheckVersionBytes(AddressType type, Network network);

// Recover address type from version byte
AddressType versionByteToAddressType(unsigned int version);

// Recover network type from version byte
Network versionByteToNetwork(unsigned int version);

// Deduce address network
//Network getNetwork(std::string & base58CheckEncodedAddress);

// Deduce address type
//AddressType getType(std::string & base58CheckEncodedAddress);

}
#endif // COIN_UTILITIES_HPP
