#include "KeyPair.hpp"
#include "Utilities.hpp"

#include <QJsonObject>

KeyPair::KeyPair() {
}

KeyPair::KeyPair(const PublicKey & pk, const PrivateKey & sk)
    : _pk(pk)
    , _sk(sk) {
}

KeyPair::KeyPair(const KeyPair& keyPair)
    : KeyPair(keyPair.pk(), keyPair.sk()) {
}

KeyPair & KeyPair::operator=(const KeyPair& rhs) {

    _pk = rhs.pk();
    _sk = rhs.sk();

    return *this;
}

KeyPair::KeyPair(const QJsonObject & json) {

    // _pk
    QString pk = Utilities::GET_STRING(json, "pk");
    _pk = PublicKey(pk);

    // _sk
    QString sk = Utilities::GET_STRING(json, "sk");
    _sk = PrivateKey(sk);
}

QJsonObject KeyPair::json() const {

    return QJsonObject {
        {"pk", _pk.toString()},
        {"sk", _sk.toString()}
    };
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

void KeyPair::setSk(const PrivateKey &sk) {
    _sk = sk;
}
