/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include <paymentchannel/UnspentBuyerRefundOutput.hpp>
#include <paymentchannel/RedeemScript.hpp>

namespace joystream {
namespace paymentchannel {

UnspentBuyerRefundOutput::UnspentBuyerRefundOutput(const Coin::KeyPair &keyPair, const RedeemScript &paychanScript, const Coin::typesafeOutPoint &outPoint, uint64_t outputValue)
    : UnspentP2SHOutput(keyPair, paychanScript.serialized(), uchar_vector(), outPoint, outputValue)
{

    //Are we the buyer(payor) or the seller(payee)?
    if(paychanScript.isPayorPublicKey(keyPair.pk())) {
        _sequenceNumber = RedeemScript::nSequence(paychanScript.lockTime());
        setOptionalData(RedeemScript::PayorOptionalData());
    } else {
        throw std::runtime_error("key does not match payor public key in payment channel script");
    }
}

UnspentBuyerRefundOutputSelector UnspentBuyerRefundOutput::buyerRefundOutputSelector() {
    return [](const int &chainType, const Coin::KeyPair &keyPair,
            const uchar_vector &script, const uchar_vector &optionalData,
            const Coin::typesafeOutPoint outPoint, const int64_t &value,
            bool &processNextSelector) -> Coin::UnspentOutput* {

        try {
            // Validate the script
            RedeemScript paychanScript = RedeemScript::deserialize(script);

            // If script is deserialised without errors it is a valid paychan script and no other
            // selector should try to process this output
            processNextSelector = false;

            if(paychanScript.isPayorPublicKey(keyPair.pk())) {
                return new UnspentBuyerRefundOutput(keyPair, paychanScript, outPoint, value);
            } else {
                // when the wallet is in the seller role of this payment channel we cannot construct an UnspentOutput
                // and add it to Coin::UnspentOutputSet, instead we need additional information about
                // the last payment made by the buyer and construct a paymentchannel::Settlement instead
            }

        } catch(std::exception & e) {
            processNextSelector = true;
        }

        return nullptr;
    };
}

uint32_t UnspentBuyerRefundOutput::spendingInputSequenceNumber() const {
    return _sequenceNumber;
}


}
}
