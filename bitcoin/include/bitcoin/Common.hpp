#ifndef BITCOIN_COMMON_HPP
#define BITCOIN_COMMON_HPP

class uchar_vector;

namespace Coin {
    class PublicKey;
}

namespace joystream {
namespace bitcoin {

    typedef std::function<uchar_vector(const Coin::PublicKey &)> RedeemScriptGenerator;
    typedef std::function<uchar_vector(const Coin::PublicKey &, uint32_t n)> MultiRedeemScriptGenerator;

}}

#endif // BITCOIN_COMMON_HPP

