#include <paymentchannel/RedeemScript.hpp>
#include <common/PublicKey.hpp>
#include <common/Utilities.hpp>

namespace joystream {
namespace paymentchannel {

const uchar_vector REDEEMSCRIPT_TEMPLATE = RedeemScript::templateScript();

RedeemScript::RedeemScript(const Coin::PublicKey & payorPk, const Coin::PublicKey & payeePk, uint32_t lockTime)
    : _payorPk(payorPk)
    , _payeePk(payeePk)
    , _lockTime(lockTime) {

}

uchar_vector RedeemScript::templateScript() {
    uchar_vector script;

    script += uchar_vector(0x63); // OP_IF
    // Branch for when channel is settled with payment and refund
    script += Coin::opPushData(0x21);
    script += uchar_vector("0000000000000000000000000000000000000000");
    script += uchar_vector(0xad); // OP_CHECKSIGVERIFY

    script += uchar_vector(0x67); // OP_ELSE
    // Branch for when channel is settled with full refund to payor
    script += Coin::opPushData(4);
    script += uchar_vector("00000000");
    script += uchar_vector(0xb1); // OP_CHECKLOCKTIMEVERIFY
    script += uchar_vector(0x75); // OP_DROP

    script += uchar_vector(0x68); // OP_ENDIF
    // Check that payor has agreed to this spend
    script += Coin::opPushData(0x21);
    script += uchar_vector("0000000000000000000000000000000000000000");
    script += uchar_vector(0xac); // OP_CHECKSIG

    return script;
}

uchar_vector RedeemScript::serialized() const {

    uchar_vector script = REDEEMSCRIPT_TEMPLATE;

    uchar_vector pk1 = _payeePk.toUCharVector();
    std::copy(pk1.begin(),pk1.end(), script.begin() + 2);

    uchar_vector locktime = Coin::serializeScriptNum(_lockTime);
    std::copy(locktime.begin(), locktime.end(), script.begin() + 40);

    uchar_vector pk2 = _payorPk.toUCharVector();
    std::copy(pk2.begin(),pk2.end(), script.begin() + 49);

    return script;
}

RedeemScript RedeemScript::deserialize(const uchar_vector & script) {
    // check size
    if(script.size() != REDEEMSCRIPT_TEMPLATE.size()) {
        throw std::runtime_error("RedeemScript Incorrect Size");
    }

    // Logically AND the script with the template which acts as a mask
    // If the result is equal to the template then its of the correct type
    for(uint n = 0; n < script.size(); n++) {
        if(script[n] & REDEEMSCRIPT_TEMPLATE[n] != REDEEMSCRIPT_TEMPLATE[n]){
            throw std::runtime_error("RedeemScript is not a paychannel script");
        }
    }

    Coin::PublicKey payee(uchar_vector(script.begin()+2, script.begin()+2+33));
    uint32_t locktime = Coin::deserializeScriptNum(uchar_vector(script.begin()+40,script.begin()+40+4));
    Coin::PublicKey payor(uchar_vector(script.begin()+49,script.begin()+49+33));

    return RedeemScript(payor, payee, locktime);

}

}}
