/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 3 2015
 */

#include <common/CoinWrappers.hpp>

#include <QByteArray>

using namespace Coin;
//using namespace Coin::Utilities;

/**
unsigned int extendedPrivateKeyVersionBytes(Network network) {

    switch(network) {

        case Network::testnet3: return 0x04358394;
        case Network::mainnet: return 0x0488ADE4;
        default:
            Q_ASSERT(false);
    }
}

unsigned int extendedPublicKeyVersionBytes(Network network) {

    switch(network) {

        case Network::testnet3: return 0x043587CF;
        case Network::mainnet: return 0x0488B21E;
        default:
            Q_ASSERT(false);
    }
}



unsigned int toBase58CheckVersionBytes(AddressType type, Network network) {

    switch(type) {

        case AddressType::PayToPublicKeyHash:

            if(network == Network::mainnet)
                return 0x0; // decimal 0
            else if(network == Network::testnet3)
                return 0x6F; // decimal 111;

            break;

        case AddressType::PayToScriptHash:

            break;

            if(network == Network::mainnet)
                return 0x5; // decimal 5
            else if(network == Network::testnet3)
                return 0xC4; // decimal 96

        default:
            Q_ASSERT(false);
    }
}

AddressType versionByteToAddressType(unsigned int version) {

    switch(version) {

        case 0x0: return AddressType::PayToPublicKeyHash;
        case 0x6F: return AddressType::PayToPublicKeyHash;
        case 0x5: return AddressType::PayToScriptHash;
        case 0xC4: return AddressType::PayToScriptHash;
        default:
            Q_ASSERT(false);
    }
}

Network versionByteToNetwork(unsigned int version) {

    switch(version) {

        case 0x0: return Network::mainnet;
        case 0x6F: return Network::testnet3;
        case 0x5: return Network::mainnet;
        case 0xC4: return Network::testnet3;
        default:
            Q_ASSERT(false);
    }
}


Network getNetwork(std::string & base58CheckEncodedAddress) {

    // do checks and testing

    // throw exception if ntohing matches
}

AddressType getType(std::string & base58CheckEncodedAddress) {

    // do checks and testing

    // throw exception if ntohing matches
}
*/

/**
QByteArray uchar_vector_to_QByteArray(const uchar_vector & v) {

    // Reinterpret from unsigned to signed char
    const char * data = reinterpret_cast<const char *>(v.data());

    // Create
    return QByteArray(data, v.size());
}
*/

/**
 * Address


Address::Address(Network network, AddressType type, const uchar_vector & payload)
    : _network(network)
    , _type(type)
    , _payload(payload) {

    // check that payload length and type field match
}

Network Address::network() const {
    return _network;
}

void Address::setNetwork(Network network) {
    _network = network;
}

AddressType Address::type() const {
    return _type;
}

void Address::setType(AddressType type) {
    _type = type;
}

uchar_vector Address::getPayload() const {
    return _payload;
}

void Address::setPayload(const uchar_vector & payload) {
    _payload = payload;
}
 */

/**
 * P2SHAddress

PSHAddress::PSHAddress(const std::vector<PublicKey> & publicKeys, uint8_t numSigsRequired, Network network)
    : Address(network) {

    std::vector::size_type totalNumberOfKeys = publicKeys.size();

    if(totalNumberOfKeys < numSigsRequired) {

        // Create error message
        std::stringstream s;

        s << totalNumberOfKeys << " keys provided, however"
          << numSigsRequired << " required.";

        throw std::runtime_error(s.c_str());
    }

    // Create multisig redeem script

    // Serialize

    // hash

    // store

}

PSHAddress::PSHAddress(const std::string & base58CheckEncoded) {

}

std::string PSHAddress::toBase58CheckEncoding() const {

}

AddressType PSHAddress::type() const {
    return AddressType::PayToScriptHash;
}
uchar_vector PSHAddress::serializedRedeemScriptHash() const
{
    return _serializedRedeemScriptHash;
}

void PSHAddress::setSerializedRedeemScriptHash(const uchar_vector &serializedRedeemScriptHash)
{
    _serializedRedeemScriptHash = serializedRedeemScriptHash;
}

*/

/**
 * Block Id

const int BlockId::length = BLOCK_ID_LENGTH;

// TxId

const unsigned int TxId::length = TXID_LENGTH;

TxId::TxId(const uchar_vector & vector) {

    // Check that vector is of correct length
    uchar_vector::size_type vectorLength = vector.size();

    if(vectorLength != TXID_LENGTH) {

        std::stringstream s;

        s << "Transaction id must be of length "
          << TXID_LENGTH
          << ", but was of length"
          << vectorLength;

        throw std::runtime_error(s.str());
    } else {
        vector.
    }

}

uchar_vector TxId::touchar_vector()() const {
    return uchar_vector(_raw.begin(), _raw.end());
}


// BlockId

const unsigned int BlockId::length = BLOCKID_BYTE_LENGTH;

// TransactionMerkleTreeRoot
const unsigned int TransactionMerkleTreeRoot::length = MERKLE_TREE_ROOT_BYTE_LENGTH;

 */
