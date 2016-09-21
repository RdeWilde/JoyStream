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

    try {
        if(!QFile::exists(storeFile)){
            wallet->create();
        } else {
            wallet->open();
        }
    } catch(std::exception & e) {
        delete wallet;
        return nullptr;
    }

    return wallet;
}

AppKit* AppKit::createInstance(const QString &dataDirectory, Coin::Network network)
{
    bitcoin::SPVWallet* wallet = getWallet(dataDirectory, network);

    if(wallet) {
        try {
            core::Node* node = core::Node::create([wallet](const Coin::Transaction &tx){
                wallet->broadcastTx(tx);
            });

            return new AppKit(node, wallet, dataDirectory);

        } catch(std::exception &e) {

        }
    }

    return nullptr;
}

AppKit::AppKit(core::Node* node, bitcoin::SPVWallet* wallet, const QString &dataDirectory)
    : _node(node),
      _wallet(wallet),
      _dataDirectory(dataDirectory) {

}

core::Node *AppKit::node() {
    return _node.get();
}

bitcoin::SPVWallet* AppKit::wallet() {
    return _wallet.get();
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

void AppKit::shutdown(const Callback & callback) {
    std::cout << "Wallet::stopSync()" << std::endl;
    _wallet->stopSync();

    std::cout << "Node::Pause()" << std::endl;
    _node->pause(callback);

    std::cout << "AppKit::Shutdown Done" << std::endl;
}

void AppKit::buyTorrent(const core::Torrent *torrent,
                        const protocol_session::BuyingPolicy& policy,
                        const protocol_wire::BuyerTerms& terms,
                        const extension::request::SubroutineHandler& handler){

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    buyTorrent(plugin, policy, terms, handler);
}

void AppKit::buyTorrent(core::TorrentPlugin *plugin,
                        const protocol_session::BuyingPolicy& policy,
                        const protocol_wire::BuyerTerms& terms,
                        const extension::request::SubroutineHandler& handler){

    // What is the best way to set this value - calculate based on buyer policy/terms ?
    const uint64_t paymentChannelFunds = 5000;

    auto outputs = _wallet->lockOutputs(paymentChannelFunds, 0, [](const uchar_vector &p2sh_redeem_script){
        // only select p2pkh outputs
        return p2sh_redeem_script.empty();
    });

    if(outputs.size() == 0) {
        // Not enough funds
        std::cout << "Not Enough Funds" << std::endl;
        return;
    }

    plugin->toBuyMode(
        // protocol_session::GenerateP2SHKeyPairCallbackHandler
        [this](const protocol_session::P2SHScriptGeneratorFromPubKey& generateScript, const uchar_vector& data) -> Coin::KeyPair {

            Coin::PrivateKey sk = _wallet->generateKey([&generateScript, &data](const Coin::PublicKey & pk){
                return bitcoin::RedeemScriptInfo(generateScript(pk), data);
            });

            return Coin::KeyPair(sk);
        },
        // protocol_session::GenerateReceiveAddressesCallbackHandler
        [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
            std::vector<Coin::P2PKHAddress> addresses;

            for(int n = 0; n < npairs; n++) {
                addresses.push_back(_wallet->generateReceiveAddress());
            }

            return addresses;
        },
        // protocol_session::GenerateChangeAddressesCallbackHandler
        [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
            std::vector<Coin::P2PKHAddress> addresses;

            for(int n = 0; n < npairs; n++) {
                addresses.push_back(_wallet->generateChangeAddress());
            }

            return addresses;
        },
        // Coin::UnspentOutputSet
        outputs,
        policy,
        terms,
        [this, outputs, handler](const std::exception_ptr & e) {
            if(e)
                _wallet->unlockOutputs(outputs);
            handler(e);
        });
}

void AppKit::buyTorrent(const libtorrent::sha1_hash &info_hash,
                        const protocol_session::BuyingPolicy& policy,
                        const protocol_wire::BuyerTerms& terms,
                        const extension::request::SubroutineHandler& handler) {

    auto torrents = _node->torrents();

    if(torrents.find(info_hash) == torrents.end())
        return;

    buyTorrent(torrents[info_hash], policy, terms, handler);
}

void AppKit::sellTorrent(core::TorrentPlugin *plugin,
                         const protocol_session::SellingPolicy &policy,
                         const protocol_wire::SellerTerms &terms,
                         const SubroutineHandler& handler){

    plugin->toSellMode(
        // protocol_session::GenerateP2SHKeyPairCallbackHandler
        [this](const protocol_session::P2SHScriptGeneratorFromPubKey& generateScript, const uchar_vector& data) -> Coin::KeyPair {

            Coin::PrivateKey sk = _wallet->generateKey([&generateScript, &data](const Coin::PublicKey & pk){
                return bitcoin::RedeemScriptInfo(generateScript(pk), data);
            });

            return Coin::KeyPair(sk);
        },
        // protocol_session::GenerateReceiveAddressesCallbackHandler
        [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
            std::vector<Coin::P2PKHAddress> addresses;

            for(int n = 0; n < npairs; n++) {
                addresses.push_back(_wallet->generateReceiveAddress());
            }

            return addresses;
        },
        policy,
        terms,
        handler);
}

void AppKit::sellTorrent(const core::Torrent *torrent,
                         const protocol_session::SellingPolicy &policy,
                         const protocol_wire::SellerTerms &terms,
                         const SubroutineHandler& handler){

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    sellTorrent(plugin, policy, terms, handler);
}
} // joystream namespace
