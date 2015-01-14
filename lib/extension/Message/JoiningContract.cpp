#include "JoiningContract.hpp"
#include "MessageType.hpp"
#include <QDataStream>

JoiningContract::JoiningContract(const PublicKey & pk)
    : _pk(pk){
}

PublicKey JoiningContract::pk() const {
    return _pk;
}

MessageType JoiningContract::messageType() const {
    return MessageType::joining_contract;
}

quint32 JoiningContract::length() const {
    return PublicKey::length();
}

void JoiningContract::write(QDataStream & stream) const {
    stream << _pk;
}
