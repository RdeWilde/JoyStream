#include "KeyPair.hpp"

#include "Utilities.hpp"

#include <QJsonObject>

KeyPair::KeyPair() {

}

KeyPair::KeyPair(const KeyPair& keyPair) {
    _pk = keyPair.pk();
    _sk = keyPair.sk();
}

KeyPair & KeyPair::operator=(const KeyPair& rhs) {
    _pk = rhs.pk();
    _sk = rhs.sk();

    return *this;
}

KeyPair::KeyPair(const PublicKey & pk, const PrivateKey & sk)
    : _pk(pk)
    , _sk(sk) {
}

KeyPair::KeyPair(const QJsonObject & json) {

    // _pk
    QString pk = Utilities::GET_STRING(json, "_pk");
    _pk = PublicKey(pk);

    // _sk
    QString sk = Utilities::GET_STRING(json, "_sk");
    _sk = PrivateKey(sk);
}

QJsonObject KeyPair::toJson() const {

    QJsonObject json;

    json["_pk"] = _pk.toString();
    json["_sk"] = _sk.toString();

    return json;
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
