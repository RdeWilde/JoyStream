#ifndef SELL_MESSAGE_HPP
#define SELL_MESSAGE_HPP

#include "ExtendedMessage.hpp"
#include <cstdint>

class SellMessage : public ExtendedMessage
{

public:

    // Unit price
    uint32_t price_;

    // Fee used
    uint32_t fee_;

    // Minimum bond required
    uint32_t minimum_;

    // Constructor
    SellMessage(uint32_t price, uint32_t fee, uint32_t minimum);

};

#endif // SELL_MESSAGE_HPP
