#ifndef BITCOIN_COMMON_HPP
#define BITCOIN_COMMON_HPP

class uchar_vector;

namespace Coin {
    class PublicKey;
}

namespace joystream {
namespace bitcoin {

    struct RedeemScriptInfo {
        RedeemScriptInfo(const uchar_vector & script) : redeemScript(script) {}
        RedeemScriptInfo(const uchar_vector & script, const uchar_vector & data) : redeemScript(script), optionalData(data) {}

        uchar_vector redeemScript;
        uchar_vector optionalData;
    };

    typedef std::function<RedeemScriptInfo(const Coin::PublicKey &)> RedeemScriptGenerator;
    typedef std::function<RedeemScriptInfo(const Coin::PublicKey &, uint32_t n)> MultiRedeemScriptGenerator;

}}

#endif // BITCOIN_COMMON_HPP

