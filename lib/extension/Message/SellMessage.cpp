#include "SellMessage.hpp"

SellMessage::SellMessage(uint32_t price, uint32_t fee, uint32_t minimum)
    : price_(price)
    , fee_(fee)
    , minimum_(minimum) {
}

SellMessage::MESSAGE_TYPES SellMessage::getType() {
    return MESSAGE_TYPES::sell;
}

uint32_t SellMessage::rawLength() {
    return 1 + 3*sizeof(uint32_t);
}

uint32_t SellMessage::toRaw(char * buffer) const {

}

