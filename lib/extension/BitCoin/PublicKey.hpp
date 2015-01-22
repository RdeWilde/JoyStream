#ifndef PUBLIC_KEY_HPP
#define PUBLIC_KEY_HPP

#include "FixedBuffer.hpp"

class QString;

class PublicKey : public FixedBuffer<23>
{
public:

    QString wif() const;

};

#endif // PUBLIC_KEY_HPP
