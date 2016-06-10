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
    script += Coin::opPushData(Coin::PublicKey::length());
    script += _payeePk.toUCharVector();
    script += uchar_vector(0xad); // OP_CHECKSIGVERIFY

    script += uchar_vector(0x67); // OP_ELSE
    // Branch for when channel is settled with full refund to payor
    uchar_vector locktime = Coin::serializeScriptNum(_lockTime);
    script += Coin::opPushData(locktime.size());
    script += locktime;
    script += uchar_vector(0xb1); // OP_CHECKLOCKTIMEVERIFY
    script += uchar_vector(0x75); // OP_DROP

    script += uchar_vector(0x68); // OP_ENDIF
    // Check that payor has agreed to this spend
    script += Coin::opPushData(Coin::PublicKey::length());
    script += _payorPk.toUCharVector();
    script += uchar_vector(0xac); // OP_CHECKSIG

    return script;
}

RedeemScript RedeemScript::deserialize(const uchar_vector & script) {
    // check size (minimum size when locktime is encoded as a single byte)
    if(script.size() < 76) {
        throw std::runtime_error("RedeemScript Incorrect Size");
    }

    uchar_vector subscript(script.begin()+2, script.end());

    uchar_vector::iterator next;

    uchar_vector rawPayeePk = Coin::popData(subscript, next);

    if(rawPayeePk.empty() || next == subscript.end()) {
        throw std::runtime_error("Unable to retreive payee public key from redeem script");
    }

    Coin::PublicKey payeePk(rawPayeePk);

    subscript = uchar_vector(next, subscript.end());
    uchar_vector rawLockTime = Coin::popData(subscript, next);

    if(rawLockTime.empty() || next == subscript.end()) {
        throw std::runtime_error("Unable to retreive Locktime from redeem script");
    }

    uint32_t locktime = Coin::deserializeScriptNum(rawLockTime);

    subscript = uchar_vector(next + 3, subscript.end());
    uchar_vector rawPayorPk = Coin::popData(subscript, next);

    if(rawPayorPk.empty() || next == subscript.end()) {
        throw std::runtime_error("Unable to retreive payor public key from redeem script");
    }

    Coin::PublicKey payorPk(rawPayorPk);

    RedeemScript result(payorPk, payeePk, locktime);

    if(result.serialized() == script) {
        return result;
    }

    throw std::runtime_error("script is not a valid paychan script");

}

}}
