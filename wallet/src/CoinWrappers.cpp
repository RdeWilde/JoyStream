#include <wallet/CoinWrappers.hpp>

using namespace Coin;

/**
 * PublicKey
 */

static const int PublicKey::compressedLength = 33;
static const int PublicKey::unCompressedLength = 65;

PublicKey::PublicKey(const uchar_vector & raw)
    : _raw(raw) {

    // Validate length
    std::vector::size_type length = _raw.size();

    if(length != PublicKey::compressedLength &&
            length != PublicKey::unCompressedLength) {

        std::stringstream s;

        s << "Invaid public key length found, must be "
          << PublicKey::compressedLength << " (compressed) or"
          << PublicKey::unCompressedLength << " (uncompressed), but found: " << length;

        throw std::runtime_error(s.c_str());
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

static int PrivateKey::length = 32;

PrivateKey::PrivateKey(const uchar_vector_secure & raw)
    : _raw(raw) {

    // Validate length
    std::vector::size_type length = _raw.size();

    // Valiate length
    if(length != PrivateKey::length) {

        std::stringstream s;

        s << "Invaid private key length found, must be "
          << PrivateKey::length << ", but found: "
          << length;

        throw std::runtime_error(s);
    }
}

uchar_vector_secure PrivateKey::raw() const {
    return _raw;
}

/**
 * Signature
 */

static int Signature::maxLength = 73;

Signature::Signature(const uchar_vector_secure & raw)
    : _raw(raw) {

    // Validate length
    std::vector::size_type length = _raw.size();

    // Check that the maximum length is not exceeded
    if(raw.size() > Signature::maxLength) {

        std::stringstream s;

        s << "Invaid signature length found, must be no longer than"
          << Signature::maxLength << ", but found: "
          << length;

        throw std::runtime_error(s);
    }
}

std::vector::size_type Signature::length() const  {
    return _raw.size();
}

uchar_vector_secure Signature::raw() const {
    return _raw;
}

/**
 * Address
 */

Address::Address(Network network)
    : _network(network) {
}

Network Address::network() const {
    return _network;
}

void Address::setNetwork(Network network) {
    _network = network;
}

/**
 * P2PKHAddress
 */

P2PKHAddress::P2PKHAddress(const PublicKey & publicKey, Network network)
    : Address(network) {

    // hash public key

    // store
}

P2PKHAddress::P2PKHAddress(const std::string & base58CheckEncoded) {

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
    _publicKeyHash = publicKeyHash;
}

/**
 * P2SHAddress
 */

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

