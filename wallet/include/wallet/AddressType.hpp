#ifndef ADDRESS_TYPE
#define ADDRESS_TYPE

enum class AddressType {
    PayToPublicKeyHash,
    //PayToPublicKey,
    PayToScriptHash
    //Multisig,

    //Data // Does this have address format
};

#include "Network.hpp"
#include <utility> // std::pair

// https://en.bitcoin.it/wiki/List_of_address_prefixes
static unsigned int toBase58CheckVersion(AddressType type, Network network);
static std::pair<AddressType, Network> versionToAddressInformation(unsigned int version);

#endif // ADDRESS_TYPE

