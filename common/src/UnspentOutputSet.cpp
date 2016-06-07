#include <common/UnspentOutputSet.hpp>

#include <CoinCore/CoinNodeData.h>

namespace Coin {

UnspentOutputSet::UnspentOutputSet()
{

}

uint64_t UnspentOutputSet::value() const {
    using namespace std;

    uint64_t total = 0;

    for(auto & utxo : *this) {
        total += utxo->value();
    }

    return total;
}

// Warning - user should have already added a change output
bool UnspentOutputSet::finance(Transaction & cointx, const SigHashType & sigHashType) const {

    // Esure we have enough coins to finance the transaction
    uint64_t totalOutputValue = 0;

    for(const Coin::TxOut & output : cointx.outputs) {
        totalOutputValue += output.value;
    }

    if(value() < totalOutputValue)
        return false; // Not Enough Funds to Finance

    // Clear inputs
    cointx.inputs.clear();

    // Setup all inputs to transaction
    for(auto & utxo : *this) {
        cointx.inputs.push_back(Coin::TxIn(utxo->outPoint().getClassicOutPoint(), uchar_vector(), 0xFFFFFFFF));
    }

    // Sign the inputs
    for(auto & input : cointx.inputs) {
        auto utxo = outputFromOutPoint(input.previousOut);
        input.scriptSig = utxo->scriptSig(cointx, sigHashType);
    }
}

std::shared_ptr<UnspentOutput> UnspentOutputSet::outputFromOutPoint(const Coin::OutPoint & outpoint) const {

    for(auto & utxo : *this) {
        if(utxo->outPoint() == typesafeOutPoint(outpoint)) return utxo;
    }

    return nullptr;
}

}
