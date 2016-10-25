#include <app_kit/AppKit.hpp>
#include <app_kit/DataDirectory.hpp>

#include <core/core.hpp>
#include <bitcoin/SPVWallet.hpp>

#include <QDir>

namespace joystream {
namespace appkit {

bitcoin::SPVWallet * AppKit::getWallet(const DataDirectory &dataDirectory, Coin::Network network) {
    auto storeFile = dataDirectory.walletFilePath();
    auto wallet = new bitcoin::SPVWallet(storeFile.toStdString(),
                                         dataDirectory.blockTreeFilePath().toStdString(),
                                         network);

    std::cout << "Looking for wallet file " << storeFile.toStdString() << std::endl;
    if(!QFile::exists(storeFile)){
        std::cout << "Wallet not found.\nCreating a new wallet..." << std::endl;
        wallet->create();
    } else {
        std::cout << "Wallet found, opening..." << std::endl;
        wallet->open();
    }

    return wallet;
}

AppKit* AppKit::create(const QString &dataDirectory, Coin::Network network, std::string host, int port)
{
    DataDirectory *dataDir = new DataDirectory(dataDirectory);

    dataDir->lock();

    bitcoin::SPVWallet* wallet = nullptr;
    core::Node* node = nullptr;

    try {

        wallet = getWallet(*dataDir, network);

        wallet->loadBlockTree();

        node = core::Node::create([wallet](const Coin::Transaction &tx){
            wallet->broadcastTx(tx); // queue TX for rebroadcast if wallet is offline
        });

    } catch(std::exception &e) {
        if(wallet) delete wallet;
        if(node) delete node;
        dataDir->unlock();
        throw e;
    }

    return new AppKit(node, wallet, dataDir, host , port);
}

AppKit::AppKit(core::Node* node, bitcoin::SPVWallet* wallet, DataDirectory *dataDirectory, std::string host, int port)
    : _node(node),
      _wallet(wallet),
      _dataDirectory(dataDirectory),
      _bitcoinHost(host),
      _bitcoinPort(port) {

    _timer = new QTimer();

    QObject::connect(_timer, &QTimer::timeout, [this](){
        _node->updateStatus();

        // try to reconnect to bitcoin network if wallet went offline
        syncWallet(_bitcoinHost, _bitcoinPort);

        //... rebroadcast transactions..
    });

    _timer->start(1000); // 1s interval
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

void AppKit::shutdown(const Callback & shutdownComplete) {
    _timer->stop();

    std::cout << "Shutting down AppKit" << std::endl;

    _node->pause([shutdownComplete, this](){
        std::cout << "Node paused" << std::endl;

        std::cout << "Stopping wallet" << std::endl;
        _wallet->stopSync();

        _dataDirectory->unlock();

        std::cout << "AppKit shutdown complete" << std::endl;
        shutdownComplete();
    });
}

void AppKit::addTorrent(const core::TorrentIdentifier &torrentReference, const core::Node::AddedTorrent &addedTorrent) {

    // addTorrent adds a torrent synchronously to libtorrent
    _node->addTorrent(0, // default upload bandwidth limit
                      0, // default download bandwidth limit
                      torrentReference.infoHash().to_string(),
                      std::vector<char>(), _dataDirectory->defaultSavePath().toStdString(),
                      false,
                      torrentReference,
                      addedTorrent);
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

    auto outputs = _wallet->lockOutputs(paymentChannelFunds, 0);

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

} // appkit namespace
} // joystream namespace
