#include "KeyPair.hpp"

KeyPair::KeyPair() {

}

KeyPair::KeyPair(const KeyPair& keyPair) {
    _pk = keyPair.pk();
    _sk = keyPair.sk();
}

KeyPair & KeyPair::operator=(const KeyPair& rhs) {
    _pk = keyPair.pk();
    _sk = keyPair.sk();
}

KeyPair::KeyPair(const PublicKey & pk, const PrivateKey & sk)
    : _pk(pk)
    , _sk(sk) {
}

PublicKey KeyPair::pk() const {
    return _pk;
}

void KeyPair::setPk(const PublicKey &pk) {
    _pk = pk;
}

PrivateKey KeyPair::sk() const {
    return _sk;
}

void KeyPair::setSk(const PrivateKey &sk)
{
    _sk = sk;
}


