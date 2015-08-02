#ifndef COIN_CORE_WRAPPERS
#define COIN_CORE_WRAPPERS

#include <stdutils/uchar_vector.h>

#include <wallet/Network.hpp>
#include <wallet/AddressType.hpp>

#include <array>

class QByteArray;

namespace Coin {

static QByteArray uchar_vector_to_QByteArray(const uchar_vector & v);

class PublicKey {

public:

    // Length of compressed public key representation
    static const int compressedLength;

    // Length of uncompressed public key representation
    static const int unCompressedLength;

    PublicKey(const uchar_vector & raw);
    PublicKey(const PublicKey & publicKey);

    bool operator==(const PublicKey & rhs);
    bool operator!=(const PublicKey & rhs);

    bool isCompressed() const;

    uchar_vector raw() const;

private:

    // Raw public key, either uncompressed or compressed
    uchar_vector _raw;
};

class PrivateKey {

public:

    // Length of raw publi key. 32
    static const int length;

    PrivateKey(const uchar_vector_secure & raw);

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

// Deduce address network
static Network getNetwork(std::string & base58CheckEncodedAddress);

// Deduce address type
static AddressType getType(std::string & base58CheckEncodedAddress);

/**
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

class P2PKHAddress {

public:

    // Length of pubkeyhash field in address (20)
    static const int pubKeyHashLength;

    // Constructor from members
    P2PKHAddress(Network network, const PublicKey & publicKey);

    // Constructor from raw encoded address
    P2PKHAddress(const uchar_vector & raw);

    // Constructor from base58check encoded string
    P2PKHAddress(const std::string & base58CheckEncoded);

    // Raw byte encoding of address
    uchar_vector raw() const;

    // base58Check encoding of address
    std::string toBase58CheckEncoding() const;

    // Type of address: remove later
    AddressType type() const;

    // Getters and setters
    uchar_vector publicKeyHash() const;
    void setPublicKeyHash(const uchar_vector & publicKeyHash);

private:

    // Network to which this address corresponds
    Network _network;

    // Bitcoin public key hash
    uchar_vector _publicKeyHash;

};

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

// (type safe length) fixed length array of unsigned chars
template<unsigned array_length>
class fixed_uchar_array : public std::array<unsigned char, array_length> {

public:

    // Construct form unsigned char vector
    fixed_uchar_array(const uchar_vector & vector);
    fixed_uchar_array(const QByteArray & byteArray);

    // Convert to unsigned char vector
    uchar_vector toUCharVector() const;
    QByteArray toByteArray() const;
}

// Byte lengths of various data types
#define TXID_BYTE_LENGTH 20
#define BLOCKID_BYTE_LENGTH 32
#define TRANSACTIONMERKLETREEROOT_BYTE_LENGTH 32

// Define Bitcoin types as spesific length fixed uchar arrays
typedef fixed_uchar_array<TXID_BYTE_LENGTH> TxId;
typedef fixed_uchar_array<BLOCKID_BYTE_LENGTH> BlockId;
typedef fixed_uchar_array<TRANSACTIONMERKLETREEROOT_BYTE_LENGTH> TransactionMerkleTreeRoot;

/**
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
