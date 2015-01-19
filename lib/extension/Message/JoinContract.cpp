#include "JoinContract.hpp"
#include "MessageType.hpp"

MessageType JoinContract::messageType() const {
    return MessageType::join_contract;
}
