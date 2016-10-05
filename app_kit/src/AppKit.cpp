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
        std::cout << "Looking for wallet file " << storeFile.toStdString() << std::endl;
        if(!QFile::exists(storeFile)){
            std::cout << "Wallet not found.\nCreating a new wallet..." << std::endl;
            wallet->create();
        } else {
            std::cout << "Wallet found, opening..." << std::endl;
            wallet->open();
        }

        return wallet;
    } catch(std::exception & e) {
        std::cout << "Wallet Error: " << e.what() << std::endl;
        delete wallet;
    }

    return nullptr;
}

AppKit* AppKit::createInstance(const QString &dataDirectory, Coin::Network network, std::string host, int port)
{
    bitcoin::SPVWallet* wallet = getWallet(dataDirectory, network);

    if(!wallet)
        return nullptr;

    core::Node* node = nullptr;

    try {
        node = core::Node::create([wallet](const Coin::Transaction &tx){
            wallet->broadcastTx(tx);
        });

    } catch(std::exception &e) {
        std::cout << "Node Error: " << e.what() << std::endl;
        return nullptr;
    }

    return new AppKit(node, wallet, dataDirectory, host , port);
}

AppKit::AppKit(core::Node* node, bitcoin::SPVWallet* wallet, const QString &dataDirectory, std::string host, int port)
    : _dataDirectory(dataDirectory),
      _node(node),
      _wallet(wallet),
      _bitcoinHost(host),
      _bitcoinPort(port) {

    syncWallet(_bitcoinHost, _bitcoinPort);
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
        } else if(_wallet->network() == Coin::Network::mainnet) {
            _wallet->sync("seed.bitcoin.sipa.be", 8333);
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

libtorrent::sha1_hash AppKit::sha1_hash_from_hex_string(const char * hex) {
  char buf[21];

  if(!libtorrent::from_hex(hex, 40, buf)){
    throw std::runtime_error("Invalid info hash string");
  }
  return libtorrent::sha1_hash(buf);
}

core::TorrentIdentifier* AppKit::makeTorrentIdentifier(const char *str)
{
    libtorrent::error_code ec;
    boost::shared_ptr<libtorrent::torrent_info> ti;

    // Is it a path to a torrent file ?
    if(QFile::exists(str)) {
        ti = boost::make_shared<libtorrent::torrent_info>(std::string(str), boost::ref(ec), 0);
        if (ec) {
            std::cerr << ec.message().c_str() << std::endl;
            return nullptr;
        } else {
            return new joystream::core::TorrentIdentifier(ti);
        }
    }

    // Is it an infohash string
    if(strlen(str) == 40) {
        try {
            ti = boost::make_shared<libtorrent::torrent_info>(sha1_hash_from_hex_string(str), 0);
            return new joystream::core::TorrentIdentifier(ti);
        } catch(std::exception &e) {
            std::cerr << "Failed to parse info hash: " << e.what() << std::endl;
        }
    }

    // Is it a magnet link ?
    try {
        auto magnetLink = joystream::core::MagnetLink::fromURI(str);
        return new joystream::core::TorrentIdentifier(magnetLink);
    } catch (std::exception &e) {
        std::cerr << "Failed to parse magnet link" << std::endl;
    }

    return nullptr;
}

} // joystream namespace
