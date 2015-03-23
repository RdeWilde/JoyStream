#include "Ready.hpp"
#include "MessageType.hpp"

Ready::Ready() {
}

MessageType Ready::messageType() const {
    return MessageType::ready;
}
