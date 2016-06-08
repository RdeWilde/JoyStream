#include <paymentchannel/RedeemScript.hpp>
#include <common/PublicKey.hpp>
#include <common/Utilities.hpp>

namespace joystream {
namespace paymentchannel {

RedeemScript::RedeemScript(const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, uint32_t lockTime)
    : _payorPk(payorPk)
    , _payeePk(payeePk)
    , _lockTime(lockTime) {

}

uchar_vector RedeemScript::serialized() const {
    uchar_vector script;

    script += uchar_vector(0x63); // OP_IF
    // Branch for when channel is settled with payment and refund
    script += Coin::opPushData(_payeePk.size());
    script += _payeePk.toUCharVector();
    script += uchar_vector(0xad); // OP_CHECKSIGVERIFY

    script += uchar_vector(0x67); // OP_ELSE
    // Branch for when channel is settled with full refund to payor
    uchar_vector lockTimeScriptNum = Coin::serializeScriptNum(_lockTime);
    script += Coin::opPushData(lockTimeScriptNum.size());
    script += lockTimeScriptNum;
    script += uchar_vector(0xb1); // OP_CHECKLOCKTIMEVERIFY
    script += uchar_vector(0x75); // OP_DROP

    script += uchar_vector(0x68); // OP_ENDIF
    // Check that payor has agreed to this spend
    script += Coin::opPushData(_payorPk.size());
    script += _payorPk.toUCharVector();
    script += uchar_vector(0xac); // OP_CHECKSIG

    return script;
}

}}
