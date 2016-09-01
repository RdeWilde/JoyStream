#include <appkit/AppKit.hpp>
#include <core/core.hpp>
#include <bitcoin/SPVWallet.hpp>

namespace joystream {


AppKit::AppKit()
{
}

std::shared_ptr<core::Node> AppKit::createAndStartNode() {
    if(_node) {
        throw std::runtime_error("Node already started");
    }

    // If there is no registered transaction broadcaster but we have a registered wallet
    // use its broadcastTx method
    if(!_registeredTransactionBroadcaster && _wallet) {
        _registeredTransactionBroadcaster = [this](const Coin::Transaction &tx) {
            if(_wallet)
                _wallet->broadcastTx(tx);
        };
    }

    _node = std::shared_ptr<core::Node>(new core::Node([this](const Coin::Transaction &tx){
        broadcastTx(tx);
    }));

    return _node;

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

void AppKit::broadcastTx(const Coin::Transaction &tx) {
    if(_registeredTransactionBroadcaster)
        _registeredTransactionBroadcaster(tx);
}

void AppKit::registerTransactionBroadcaster(const core::BroadcastTransaction &broadcastTransaction) {
    _registeredTransactionBroadcaster = broadcastTransaction;
}

void AppKit::stopBroadcastingTransactions() {
    _registeredTransactionBroadcaster = nullptr;
}


} // joystream namespace
