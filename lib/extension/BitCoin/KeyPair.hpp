#ifndef KEYPAIR_HPP
#define KEYPAIR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"

class QJsonObject;

class KeyPair
{
public:

    // Default constructor
    KeyPair();

    // Constructor from members
    KeyPair(const PublicKey & pk, const PrivateKey & sk);

    // Copy constructor
    KeyPair(const KeyPair& keyPair);

    // Assignment operator
    KeyPair & operator=(const KeyPair& rhs);

    // Constructor from json
    KeyPair(const QJsonObject & json);

    // Comparison operator
    bool operator==(const KeyPair & o) const;

    // Converts to json
    QJsonObject json() const;

    // Getters and setters
    PublicKey pk() const;
    void setPk(const PublicKey &pk);

    PrivateKey sk() const;
    void setSk(const PrivateKey &sk);

private:

    // Public key
    PublicKey _pk;

    // Private key
    PrivateKey _sk;
};

#endif // KEYPAIR_HPP
