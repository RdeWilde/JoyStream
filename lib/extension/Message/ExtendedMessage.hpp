#ifndef EXTENDED_MESSAGE_HPP
#define EXTENDED_MESSAGE_HPP

#include <cstdint>
#include <vector>

class ExtendedMessage
{
public:



    // Conversion methods back and forth
    static uint8_t messageIDFromType(MESSAGE_TYPES type);
    static MESSAGE_TYPES typeFromMessageID(uint8_t messageID);

    // Message factory
    static ExtendedMessage * fromRaw(char * buffer, uint32_t length);

    /**
     * Virtual methods that subclassing messages have to implement
     */

    // Get type of message
    virtual MESSAGE_TYPES getType();

    // Compute raw byte length
    virtual int rawLength() const = 0;

    // Write raw form to buffer
    virtual int toRaw(char * buffer) const = 0;

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
