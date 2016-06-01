#include <common/P2PKOutput.hpp>
#include <common/P2PKScriptPubKey.hpp>
#include <common/TransactionSignature.hpp>
#include <common/Utilities.hpp>

namespace Coin {

P2PKOutput::P2PKOutput()
{

}

P2PKOutput::P2PKOutput(const UnspentOutput &output)
    : UnspentOutput(output)
{
    Coin::P2PKScriptPubKey script(keyPair().pk());

    if(redeemScript() != script.serialize()) {
        throw std::runtime_error("output doesn't use a p2pk redeem script");
    }
}

uchar_vector P2PKOutput::getScriptSig(const TransactionSignature &sig) const {
    uchar_vector scriptSig;

    scriptSig += sig.opPushForScriptSigSerialized();
    scriptSig += opPushData(redeemScript().size());
    scriptSig += redeemScript();

    return scriptSig;
}

}
