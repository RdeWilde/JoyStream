#ifndef PAYMENT_CHANNEL_REDEEMSCRIPT_H
#define PAYMENT_CHANNEL_REDEEMSCRIPT_H

#include <common/PublicKey.hpp>
#include <stdutils/uchar_vector.h>

namespace Coin {
    class PublicKey;
}

namespace joystream {
namespace paymentchannel {

class RedeemScript
{
public:
    explicit RedeemScript(const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, uint32_t lockTime);

    uchar_vector serialized() const;

private:
    Coin::PublicKey _payorPk;
    Coin::PublicKey _payeePk;
    uint32_t _lockTime;
};

}}
#endif // PAYMENTCHANNEL_REDEEMSCRIPT_H
