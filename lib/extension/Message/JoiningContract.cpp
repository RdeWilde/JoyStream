#include "JoiningContract.hpp"
#include "MessageType.hpp"

#include <QDataStream>

JoiningContract::JoiningContract() {

}

JoiningContract::JoiningContract(const PublicKey & contractPk, const PublicKey & finalPk)
    : _contractPk(contractPk)
    , _finalPk(finalPk) {
}

PublicKey JoiningContract::contractPk() const {
    return _contractPk;
}

PublicKey JoiningContract::finalPk() const {
    return _finalPk;
}

MessageType JoiningContract::messageType() const {
    return MessageType::joining_contract;
}

quint32 JoiningContract::length() const {
    return PublicKey::length;
}

void JoiningContract::write(QDataStream & stream) const {
    stream << _contractPk << _finalPk;
}
