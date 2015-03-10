#ifndef KEYPAIR_HPP
#define KEYPAIR_HPP

#include "extension/BitCoin/PublicKey.hpp"
#include "extension/BitCoin/PrivateKey.hpp"

class KeyPair
{
public:

    /**
     * Default/Copy constructor and assignemtn operator needed to put in container.
     */
    KeyPair();
    KeyPair(const KeyPair& keyPair);
    KeyPair & operator=(const KeyPair& rhs);

    // Constructor
    KeyPair(const PublicKey & pk, const PrivateKey & sk);

    // Constructor from json
    KeyPair(const QJsonObject & json);

    QJsonObject toJson() const;

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
