#include "Ready.hpp"
#include "MessageType.hpp"

MessageType Ready::messageType() const {
    return MessageType::ready;
}
