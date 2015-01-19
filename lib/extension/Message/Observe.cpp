#include "Observe.hpp"
#include "MessageType.hpp"

MessageType Observe::messageType() const {
    return MessageType::observe;
}
