#ifndef COIN_UNSPENT_OUTPUT_SET_H
#define COIN_UNSPENT_OUTPUT_SET_H

#include <common/UnspentOutput.hpp>
#include <set>

namespace Coin {
                            //revert to std::set base class
class UnspentOutputSet : public std::list<std::shared_ptr<Coin::UnspentOutput>>
{
public:
    UnspentOutputSet();

    Transaction & finance(Transaction & tx, const SigHashType & sigHashType) const;

    // Sum of all outputs
    uint64_t value() const;

private:
    std::shared_ptr<UnspentOutput> outputFromOutPoint(const Coin::OutPoint & outpoint) const;
};

}
#endif // COIN_UNSPENT_OUTPUT_SET_H
