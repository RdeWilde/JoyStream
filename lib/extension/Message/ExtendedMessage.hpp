#ifndef EXTENDEDMESSAGE_HPP
#define EXTENDEDMESSAGE_HPP

#include <vector>

class ExtendedMessage
{
public:

    ExtendedMessage(int messageID);

    virtual int toRaw(std::vector<char> & buffer) const;

    virtual int rawLength() const;

private:

    int messageID_;
};

#endif // EXTENDEDMESSAGE_HPP
