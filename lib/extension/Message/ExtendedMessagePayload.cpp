#include "ExtendedMessagePayload.hpp"
#include "MessageType.hpp"
#include "Observe.hpp"
#include "Buy.hpp"
#include "Sell.hpp"

#include <QDataStream>

ExtendedMessagePayload * ExtendedMessagePayload::fromRaw(MessageType type, QDataStream & stream) {

    try {

        switch(type) {

            case MessageType::observe: return new Observe();
            case MessageType::buy: return new Buy(stream);
            case MessageType::end: return new Sell(stream);

            return NULL;
        }
    } catch (std::exception & e) {
        return NULL;
    }
}
