#include "JoinContractMessage.hpp"
#include "MessageType.hpp"

#include <QDataStream>

MessageType JoinContractMessage::messageType() const {
    return MessageType::join_contract;
}

quint32 JoinContractMessage::length() const {
    return 0;
}

void JoinContractMessage::write(QDataStream & stream) const {
}
