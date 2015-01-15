#include "JoinContract.hpp"
#include "MessageType.hpp"

#include <QDataStream>

MessageType JoinContract::messageType() const {
    return MessageType::join_contract;
}

quint32 JoinContract::length() const {
    return 0;
}

void JoinContract::write(QDataStream & stream) const {
}
