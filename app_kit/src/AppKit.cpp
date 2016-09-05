#include <app_kit/AppKit.hpp>
#include <core/core.hpp>
#include <bitcoin/SPVWallet.hpp>

#include <QDir>

namespace joystream {


AppKit::AppKit()
{
}

bitcoin::SPVWallet * AppKit::getWallet(const QString & dataDirectory, Coin::Network network) {
    QString storeFile = dataDirectory + QDir::separator() + "store.sqlite";
    QString blockTreeFile = dataDirectory + QDir::separator() + "blocktree.dat";

    auto wallet = new bitcoin::SPVWallet(storeFile.toStdString(), blockTreeFile.toStdString(), network);

    if(!QFile::exists(storeFile)){
        wallet->create();
    } else {
        wallet->open();
    }

    return wallet;
}

AppKit* AppKit::createInstance(const QString &dataDirectory, Coin::Network network)
{
    auto walletp = getWallet(dataDirectory, network);

    std::unique_ptr<bitcoin::SPVWallet> wallet(walletp);

    std::unique_ptr<core::Node> node(new core::Node([walletp](const Coin::Transaction &tx){
        walletp->broadcastTx(tx);
    }));

    return new AppKit(node, wallet, dataDirectory);

}

AppKit::AppKit(std::unique_ptr<core::Node> &node, std::unique_ptr<bitcoin::SPVWallet> &wallet, const QString &dataDirectory)
    : _dataDirectory(dataDirectory)
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

void AppKit::syncWallet(std::string host, int port) {
    if(host.empty()) {
        if(_wallet->network() == Coin::Network::testnet3) {
            _wallet->sync("testnet-seed.bitcoin.petertodd.org", 18333);
        }else{
            throw std::runtime_error("No host provided to sync wallet");
        }
    } else {
        _wallet->sync(host, port);
    }
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
