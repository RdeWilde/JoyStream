/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <paymentchannel/UnspentBuyerRefundOutput.hpp>
#include <paymentchannel/RedeemScript.hpp>

namespace joystream {
namespace paymentchannel {

UnspentBuyerRefundOutput::UnspentBuyerRefundOutput(const Coin::KeyPair &keyPair, const uchar_vector &redeemScript, const Coin::typesafeOutPoint &outPoint, uint64_t outputValue)
    : UnspentP2SHOutput(keyPair, redeemScript, uchar_vector(), outPoint, outputValue)
{
    // Validate the script
    RedeemScript paychanScript = RedeemScript::deserialize(redeemScript);

    //Are we the buyer(payor) or the seller(payee)?
    if(paychanScript.isPayorPublicKey(keyPair.pk())) {
        _sequenceNumber = RedeemScript::nSequence(paychanScript.lockTime());
        setOptionalData(RedeemScript::PayorOptionalData());
    } else {
        throw std::runtime_error("key does not match payor public key in payment channel script");
    }
}

uint32_t UnspentBuyerRefundOutput::spendingInputSequenceNumber() const {
    return _sequenceNumber;
}


}
}
