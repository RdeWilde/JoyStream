#include <wallet/CoinWrappers.hpp>

#include <QByteArray>

#include <sstream> // stringstream

using namespace Coin;

QByteArray uchar_vector_to_QByteArray(const uchar_vector & v) {

    // Reinterpret from unsigned to signed char
    const char * data = reinterpret_cast<const char *>(v.data());

    // Create
    return QByteArray(data, v.size());
}

/**
 * fixed_uchar_array
 */

template<unsigned array_length>
fixed_uchar_array<array_length>::fixed_uchar_array(const uchar_vector & vector) {

    const char * start = vector.data();
    fill(static_cast<const unsigned char *>(start), vector.size());
}

template<unsigned array_length>
fixed_uchar_array<array_length>::fixed_uchar_array(const QByteArray & byteArray) {

    const char * start = byteArray.constData();
    fill(static_cast<const unsigned char *>(start), byteArray.size());
}

template<unsigned array_length>
void fixed_uchar_array<array_length>::fill(const unsigned char * start, int length) {

    if(length != array_length) {

        std::stringstream s;

        s << "Required "
          << array_length
          << " bytes, but was provided"
          << length;

        throw std::runtime_error(s.str());
    } else {

        // Copy content into array
        for(int i = 0;i < array_length;i++)
            this[i] = start[i];
    }
}

template<unsigned array_length>
uchar_vector fixed_uchar_array<array_length>::toUCharVector() const {

    // Get pointer to data
    const unsigned char * data = static_cast<const unsigned char *>(this->data());

    // Construct vector and return it
    return uchar_vector(data, array_length);
}

template<unsigned array_length>
QByteArray fixed_uchar_array<array_length>::toByteArray() const {

    // Get pointer to data
    const char * data = reinterpret_cast<const char *>(this->data());

    // Construct byte array and return it
    return QByteArray(data, array_length);
}

/**
 * PublicKey
 */

const int PublicKey::compressedLength = 33;
const int PublicKey::unCompressedLength = 65;

PublicKey::PublicKey(const uchar_vector & raw)
    : _raw(raw) {

    // Validate length
    std::vector<unsigned char>::size_type length = _raw.size();

    if(length != PublicKey::compressedLength &&
            length != PublicKey::unCompressedLength) {

        std::stringstream s;

        s << "Invaid public key length found, must be "
          << PublicKey::compressedLength << " (compressed) or"
          << PublicKey::unCompressedLength << " (uncompressed), but found: " << length;

        throw std::runtime_error(s.str());
    }
}

PublicKey::PublicKey(const PublicKey & publicKey)
    : _raw(publicKey.raw()) {
}

bool PublicKey::operator==(const PublicKey & rhs) {
}

bool PublicKey::operator!=(const PublicKey & rhs) {

}

bool PublicKey::isCompressed() const {
    return _raw.size() == PublicKey::compressedLength;
}

uchar_vector PublicKey::raw() const {
    return _raw;
}

/**
 * PrivateKey
 */

const int PrivateKey::length = 32;

PrivateKey::PrivateKey(const uchar_vector_secure & raw)
    : _raw(raw) {

    // Validate length
    std::vector<unsigned char>::size_type length = _raw.size();

    // Valiate length
    if(length != PrivateKey::length) {

        std::stringstream s;

        s << "Invaid private key length found, must be "
          << PrivateKey::length << ", but found: "
          << length;

        throw std::runtime_error(s.str());
    }
}

PrivateKey::~PrivateKey() {

    // Clear memory
    for(uchar_vector_secure::iterator i = _raw.begin(),
        end = _raw.end();
        i != end;
        i++)
        *i = 0;

}


uchar_vector_secure PrivateKey::raw() const {
    return _raw;
}

/**
 * Signature
 */

const int Signature::maxLength = 73;

Signature::Signature(const uchar_vector_secure & raw)
    : _raw(raw) {

    // Validate length
    std::vector<unsigned char>::size_type length = _raw.size();

    // Check that the maximum length is not exceeded
    if(raw.size() > Signature::maxLength) {

        std::stringstream s;

        s << "Invaid signature length found, must be no longer than"
          << Signature::maxLength << ", but found: "
          << length;

        throw std::runtime_error(s.str());
    }
}

std::vector<unsigned char>::size_type Signature::length() const  {
    return _raw.size();
}

uchar_vector_secure Signature::raw() const {
    return _raw;
}



Network getNetwork(std::string & base58CheckEncodedAddress) {

    // do checks and testing

    // throw exception if ntohing matches
}

AddressType getType(std::string & base58CheckEncodedAddress) {

    // do checks and testing

    // throw exception if ntohing matches
}

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
 * P2PKHAddress
 */

const int P2PKHAddress::pubKeyHashLength = 20;

P2PKHAddress::P2PKHAddress(Network network, const PublicKey & publicKey)
    : _network(network) {

    // hash public key, and store in _publicKeyHash
}

P2PKHAddress::P2PKHAddress(const uchar_vector & raw) {
    // decode network and pubkey hash, and also do checksum validation
}

P2PKHAddress::P2PKHAddress(const std::string & base58CheckEncoded) {
    // Decode string to byte form
    // Call raw ctr version.
}

std::string P2PKHAddress::toBase58CheckEncoding() const {

}

AddressType P2PKHAddress::type() const {
    return AddressType::PayToPublicKeyHash;
}

uchar_vector P2PKHAddress::publicKeyHash() const {
    return _publicKeyHash;
}

void P2PKHAddress::setPublicKeyHash(const uchar_vector & publicKeyHash) {

    // Checkt the length of argument

    _publicKeyHash = publicKeyHash;
}

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
