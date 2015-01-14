#ifndef SIGN_REFUND_HPP
#define SIGN_REFUND_HPP

#include "ExtendedMessagePayload.hpp"
#include "extension/BitCoin/Hash.hpp"
#include "extension/BitCoin/PublicKey.hpp"

#include <QtGlobal>

class SignRefund : public ExtendedMessagePayload
{
public:
    SignRefund(const Hash & hash, quint32 index, quint64 value, const PublicKey & pk);

    /**
     * Virtual methods that subclassing messages have to implement
     */
    MessageType messageType() const;
    quint32 length() const;
    void write(QDataStream & stream) const;

private:

    // Contract hash
    Hash _hash;

    // Contract output
    quint32 _index;

    // Contract output value
    quint64 _value;

    // Contract output buyer multisig key
    PublicKey _pk;
};

#endif // SIGN_REFUND_HPP
