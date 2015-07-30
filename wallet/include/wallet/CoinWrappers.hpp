#ifndef COIN_CORE_WRAPPERS
#define COIN_CORE_WRAPPERS

#include <stdutils/uchar_vector.h>

#include <wallet/Network.hpp>
#include <wallet/AddressType.hpp>

class QByteArray;

namespace Coin {

static QByteArray uchar_vector_to_QByteArray(const uchar_vector & v);

template



class fixed_length_uchar_vector : public uchar_vector {

};

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

    // Deduce address type
    static Network getNetwork(std::string & base58CheckEncodedAddress);
    static AddressType getType(std::string & base58CheckEncodedAddress);

    // Base58CheckEncode
    std::string toBase58CheckEncoding();

    // Getters and setters
    Network network() const;
    void setNetwork(Network network);

    AddressType type() const;
    void setType(AddressType type);

    uchar_vector getPayload() const;
    void setPayload(const uchar_vector &payload);

protected:

    // Network to which this address corresponds
    Network _network;

    // Type of address
    AddressType _type;

    // Raw address payload
    uchar_vector _payload;
};

class P2PKHAddress : public Address {

public:

    P2PKHAddress(const PublicKey & publicKey, Network network);
    P2PKHAddress(const std::string & base58CheckEncoded);

    virtual std::string toBase58CheckEncoding() const;
    virtual AddressType type() const;

    // Getters and setters
    uchar_vector publicKeyHash() const;
    void setPublicKeyHash(const uchar_vector &publicKeyHash);

private:

    // Bitcoin public key hash
    uchar_vector _publicKeyHash;

};

/**
class PSHAddress : public Address {

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

class TxId {

public:


private:

    // Transaction id
    uchar_vector _txId;
};

}
#endif // COIN_CORE_WRAPPERS
