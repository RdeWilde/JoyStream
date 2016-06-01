#ifndef COIN_P2PKOUTPUT_H
#define COIN_P2PKOUTPUT_H

#include <common/UnspentOutput.hpp>

namespace Coin {

class P2PKOutput : public UnspentOutput
{
public:
    P2PKOutput();

    // Instance from base output type
    P2PKOutput(const KeyPair & keypair, const typesafeOutPoint & outpoint, quint64 setvalue, uchar_vector scriptpubkey, uchar_vector redeemscript);

    // P2PK redeem scripts are immediately spendable
    bool spendable() const { return true; }

    uchar_vector getScriptSig(const TransactionSignature &sig) const;
};

}
#endif // COIN_P2PKOUTPUT_H
