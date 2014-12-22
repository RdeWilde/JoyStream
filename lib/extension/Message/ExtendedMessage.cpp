#include "ExtendedMessage.hpp"
#include "PassiveMessage.hpp"
#include "BuyMessage.hpp"
#include "SellMessage.hpp"

ExtendedMessage * ExtendedMessage::fromRaw(MessageType type, QDataStream & extendedPayloadStream) {

    try {

        switch(type) {

            case MessageType::passive: return new PassiveMessage();
            case MessageType::buy: return new BuyMessage();
            case MessageType::end: return new SellMessage(extendedPayloadStream);

        }
    } catch (std::exception & e) {
        return NULL;
    }
}
