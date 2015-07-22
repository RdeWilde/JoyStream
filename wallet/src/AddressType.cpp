
#include <wallet/AddressType.hpp>

// https://en.bitcoin.it/wiki/List_of_address_prefixes
unsigned int toBase58CheckVersion(AddressType type, Network network) {

    switch(type) {

        case AddressType::PayToPublicKeyHash:

            if(network == Network::mainnet)
                return 0x0; // decimal 0
            else if(network == Network::testnet3)
                return 0x6F; // decimal 111;

            break;

        case AddressType::PayToScriptHash:
            break;

            if(network == Network::mainnet)
                return 0x5; // decimal 5
            else if(network == Network::testnet3)
                return 0xC4; // decimal 96

        default:
    }
}

static std::pair<AddressType, Network> versionToAddressInformation(unsigned int version)  {

    switch(version) {

        case 0x0: return std::make_pair(AddressType::PayToPublicKeyHash, Network::mainnet);
        case 0x6F: return std::makepair(AddressType::PayToPublicKeyHash, Network::testnet3);
        case 0x5: return std::makepair(AddressType::PayToScriptHash, Network::mainnet);
        case 0xC4: return std::makepair(AddressType::PayToScriptHash, Network::testnet3);
        default:
    }
}

