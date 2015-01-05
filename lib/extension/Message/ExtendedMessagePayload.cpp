#include "ExtendedMessagePayload.hpp"
#include "MessageType.hpp"
#include "ObserveMessage.hpp"
#include "BuyMessage.hpp"
#include "SellMessage.hpp"

#include <QDataStream>

ExtendedMessagePayload * ExtendedMessagePayload::fromRaw(MessageType type, QDataStream & stream) {

    try {

        switch(type) {

            case MessageType::observe: return new ObserveMessage();
            case MessageType::buy: return new BuyMessage(stream);
            case MessageType::end: return new SellMessage(stream);

            return NULL;
        }
    } catch (std::exception & e) {
        return NULL;
    }
}
