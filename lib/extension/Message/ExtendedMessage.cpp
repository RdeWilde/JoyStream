#include "ExtendedMessage.hpp"
#include "BuyMessage.hpp"
#include "SellMessage.hpp"

ExtendedMessage * ExtendedMessage::fromRaw(Message::TYPE type, QDataStream & extendedPayloadStream) {

    //try {

        switch(type) {

            case Message::buy: return new BuyMessage();
            case Message::end: return new SellMessage(extendedPayloadStream);

        }
    //} catch (std::exception & e) {
    //    return 0;
    //}
}
