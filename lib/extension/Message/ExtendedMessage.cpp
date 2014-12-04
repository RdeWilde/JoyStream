#include "ExtendedMessage.hpp"
#include "BuyMessage.hpp"
#include "SellMessage.hpp"

ExtendedMessage * ExtendedMessage::fromRaw(MessageType type, QDataStream & extendedPayloadStream) {

    //try {

        switch(type) {

            case MessageType::buy: return new BuyMessage();
            case MessageType::end: return new SellMessage(extendedPayloadStream);

        }
    //} catch (std::exception & e) {
    //    return 0;
    //}
}
