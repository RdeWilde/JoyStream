#include <appkit/AppKit.hpp>
#include <core/core.hpp>
#include <bitcoin/SPVWallet.hpp>

namespace joystream {


AppKit::AppKit()
{
}

bitcoin::SPVWallet * AppKit::getWallet(std::string dataDirectory, Coin::Network network) {
    std::string storeFile = dataDirectory + "/store.sqlite";
    std::string blockTreeFile = dataDirectory + "/blocktree.dat";

    return new bitcoin::SPVWallet(storeFile, blockTreeFile, network);
}

void AppKit::createWallet(std::string dataDirectory, Coin::Network network) {

    std::unique_ptr<bitcoin::SPVWallet> wallet(getWallet(dataDirectory, network));

    wallet->create();
}

AppKit* AppKit::createInstance(std::string dataDirectory, Coin::Network network) {

    auto walletp = getWallet(dataDirectory, network);

    std::unique_ptr<bitcoin::SPVWallet> wallet(walletp);

    wallet->open();

    std::unique_ptr<core::Node> node(new core::Node([walletp](const Coin::Transaction &tx){
        walletp->broadcastTx(tx);
    }));

    return new AppKit(node, wallet);

}

AppKit::AppKit(std::unique_ptr<core::Node> &node, std::unique_ptr<bitcoin::SPVWallet> &wallet)
{
    _node = std::move(node);
    _wallet = std::move(wallet);
}

std::unique_ptr<core::Node>& AppKit::node() {
    return _node;
}

std::unique_ptr<bitcoin::SPVWallet>& AppKit::wallet() {
    return _wallet;
}

void AppKit::buyTorrent(std::shared_ptr<core::Torrent> &torrent,
                        protocol_session::BuyingPolicy& policy,
                        protocol_wire::BuyerTerms& terms,
                        extension::request::SubroutineHandler& handler){

    // This will be termporary. funding of the contract will be done
    // by an external transaction signing/funding routine
    auto outputs = _wallet->lockOutputs(1000, 0);

    if(outputs.size() == 0)
        return;

    if(outputs.size() > 1){
        _wallet->unlockOutputs(outputs);
        return;
    }

    torrent->torrentPlugin()->toBuyMode(
        // protocol_session::GenerateKeyPairsCallbackHandler
        [this](int npairs){
            return _wallet->getKeyPairs(npairs, false);
        },
        // protocol_session::GenerateP2PKHAddressesCallbackHandler
        [this](int npairs){
            auto keyPairs = _wallet->getKeyPairs(npairs, true);
            std::vector<Coin::P2PKHAddress> addresses;
            for(const Coin::KeyPair &keyPair : keyPairs) {
                addresses.push_back(Coin::P2PKHAddress(_wallet->network(), keyPair.pk().toPubKeyHash()));
            }
            return addresses;
        },
        // Coin::UnspentP2PKHOutput
        outputs.front(),
        policy,
        terms,
        [this, outputs, &handler](const std::exception_ptr & e) {
            _wallet->unlockOutputs(outputs);
            handler(e);
        }

    );
}

void AppKit::buyTorrent(libtorrent::sha1_hash &info_hash,
                        protocol_session::BuyingPolicy& policy,
                        protocol_wire::BuyerTerms& terms,
                        extension::request::SubroutineHandler& handler) {

    auto torrents = _node->torrents();

    if(torrents.find(info_hash) == torrents.end())
        return;

    buyTorrent(torrents[info_hash], policy, terms, handler);
}


} // joystream namespace
