#ifndef EXTENDED_MESSAGE_HPP
#define EXTENDED_MESSAGE_HPP

#include <cstdint>
#include <vector>

class ExtendedMessage
{
public:

    // Menu of extended message types in protocol
    enum MESSAGE_TYPES {
        buy,
        sell,
        setup_begin,
        setup_begin_reject,
        setup_contract,
        setup_contract_signed,
        setup_refund,
        setup_refund_signed,
        setup_contract_published,
        setup_completed,
        piece_get,
        piece_put,
        payment,
        end
    };

    // Conversion methods back and forth
    static uint8_t messageIDFromType(MESSAGE_TYPES type);
    static MESSAGE_TYPES typeFromMessageID(uint8_t messageID);

    // Message factory
    static ExtendedMessage * fromRaw(char * buffer, uint32_t length);

    /**
     * Virtual methods that subclassing message classes have to implement
     */

    // Get type of message
    virtual MESSAGE_TYPES getType();

    // Compute raw byte length
    virtual uint32_t rawLength() const = 0;

    // Write raw form to buffer
    virtual uint32_t toRaw(char * buffer) const = 0;

/*
private:

    // Extended message id
    uint8_t messageID_;


protected:

    // Protected constructor used by subclassing messages
    ExtendedMessage(uint8_t messageID);
    */
};

#endif // EXTENDED_MESSAGE_HPP
