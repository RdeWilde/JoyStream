/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#ifndef COIN_CORE_WRAPPERS
#define COIN_CORE_WRAPPERS

#include <stdint.h>

#include <common/Network.hpp>
#include <common/AddressType.hpp>

class QByteArray;

namespace Coin {

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

// Convert uchar vector to bytearray
//QByteArray uchar_vector_to_QByteArray(const uchar_vector & v);

/**
class PrivateKey {

public:

    // Length of raw publi key. 32
    static const int length;

    PrivateKey(const uchar_vector_secure & raw);

    // Safe private key destructor
    ~PrivateKey();

    // Destructor which zeros out _raw

    uchar_vector_secure raw() const;

private:

    // Raw private key
    uchar_vector_secure _raw;
};

class Signature  {

public:

    // Question: is DER encoding natural for
    // btc signatures, ask Codeshark

    // Maximum length
    static const int maxLength;

    Signature(const uchar_vector_secure & raw);

    // Destructor which zeros out _raw

    // Length of signature
    std::vector<unsigned char>::size_type length() const;

    // Getters and setters
    uchar_vector_secure raw() const;

private:

    // DER encoded Bitcoin signature
    uchar_vector_secure _raw;
};


class Address {

public:

    Address(Network network, const uchar_vector & payload);

    // Base58CheckEncode
    virtual std::string toBase58CheckEncoding() = 0;

    // Getters and setters
    Network network() const;
    void setNetwork(Network network);

    virtual AddressType type() const = 0;

protected:

    // Network to which this address corresponds
    Network _network;
};
*/

/**
class PSHAddress {

public:

    // Create multisig p2sh
    PSHAddress(const std::vector<PublicKey> & publicKey, int numSigsRequired, Network network);

    // Decode p2sh address from base58check encoded address string
    PSHAddress(const std::string & base58CheckEncoded);

    virtual std::string toBase58CheckEncoding() const;
    virtual AddressType type() const;

    // Getters and setters
    uchar_vector serializedRedeemScriptHash() const;
    void setSerializedRedeemScriptHash(const uchar_vector & serializedRedeemScriptHash);

private:

    // Hash of serialized redeem script which
    // controls redemption of p2sh output
    uchar_vector _serializedRedeemScriptHash;
};
*/

/**
 * old style
typedef std::array<unsigned char, TXID_BYTE_LENGTH> TxId;
typedef std::array<unsigned char, BLOCKID_BYTE_LENGTH> BlockId;
typedef std::array<unsigned char, TRANSACTIONMERKLETREEROOT_BYTE_LENGTH> TransactionMerkleTreeRoot;

// Turn uchar_vector into std::array of given length
template<unsigned array_length>
std::array<unsigned char, array_length> toArray(const uchar_vector & vector);

// Turn std::array of given length into uchar_vector
template<unsigned array_length>
uchar_vector toUCharVector(const std::array<unsigned char, array_length> & array);

// Include implementations of templated functions,
// needed due to C++ templating madness
#include "src/CoinWrappers_templated_functions.cpp"
*/

/**
class TxId : public std::array<unsigned char, TXID_LENGTH> {

public:

    // Byte length of transaction id
    static const unsigned int length;

    // Constructor from member
    TxId(const uchar_vector & vector);

    // Get as unsigned character vector
    uchar_vector touchar_vector() const;

private:

    // Transaction raw byte array
     _raw;
};


class TxId {

public:

    // Byte length of transaction id
    static const unsigned int length;

    // Constructor from member
    TxId(const uchar_vector & vector);

    // Get as unsigned character vector
    uchar_vector touchar_vector() const;

private:

    // Transaction raw byte array
    std::array<unsigned char, TXID_LENGTH> _raw;
};


// Id of block, i.e. hash of block header
class BlockId {

public:

    // Byte length of block id
    static const unsigned int length;

    BlockId

private:

    // Raw byte array representation of block id
    std::array<unsigned char, BLOCKID_BYTE_LENGTH> _raw;
};

// Same as above only different length
class TransactionMerkleTreeRoot {

public:

    // Byte length of merkle tree root
    static const unsigned int length;

    TransactionMerkleTreeRoot(const uchar_vector & vector);



private:

    // Raw byte array represetnation of merkle tree root
    std::array<unsigned char, MERKLE_TREE_ROOT_BYTE_LENGTH> _raw;
};
*/

}

#endif // COIN_CORE_WRAPPERS
