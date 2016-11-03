#include <app_kit/AppKit.hpp>
#include <app_kit/DataDirectory.hpp>
#include <app_kit/Settings.hpp>
#include <app_kit/SavedTorrents.hpp>
#include <app_kit/SavedTorrentParameters.hpp>

#include <core/core.hpp>
#include <bitcoin/SPVWallet.hpp>

#include <libtorrent/create_torrent.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

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

AppKit* AppKit::create(std::string dataDirectoryPath, Coin::Network network, const Settings &settings)
{
    DataDirectory *dataDir = new DataDirectory(QString::fromStdString(dataDirectoryPath));

    dataDir->lock();

    bitcoin::SPVWallet* wallet = nullptr;
    core::Node* node = nullptr;
    TransactionSendBuffer *sendQueue = nullptr;
    try {

        wallet = getWallet(*dataDir, network);

        wallet->loadBlockTree();

        sendQueue = new TransactionSendBuffer(wallet);

        node = core::Node::create([sendQueue](const Coin::Transaction &tx){
            sendQueue->insert(tx);
        });

    } catch(std::exception &e) {
        if(node) delete node;
        if(sendQueue) delete sendQueue;
        if(wallet) delete wallet;
        dataDir->unlock();
        throw e;
    }

    return new AppKit(node, wallet, sendQueue, dataDir, settings);
}

AppKit::AppKit(core::Node* node, bitcoin::SPVWallet* wallet, TransactionSendBuffer *txSendBuffer, DataDirectory *dataDirectory, const Settings &settings)
    : _node(node),
      _wallet(wallet),
      _dataDirectory(dataDirectory),
      _settings(settings),
      _transactionSendBuffer(txSendBuffer),
      _shuttingDown(false) {

    _timer = new QTimer();

    QObject::connect(_timer, &QTimer::timeout, [this](){
        if(_shuttingDown)
            return;

        _node->updateStatus();

        // Try to reconnect to bitcoin network if wallet went offline
        syncWallet();

        // Sendout queued transactions
        _transactionSendBuffer->flush();
    });

    _timer->start(5000);
}

core::Node *AppKit::node() {
    return _node.get();
}

bitcoin::SPVWallet* AppKit::wallet() {
    return _wallet.get();
}

void AppKit::syncWallet() {
    if(_settings.bitcoinNodeHost.empty()) {
        if(_wallet->network() == Coin::Network::testnet3) {
            _wallet->sync("testnet-seed.bitcoin.petertodd.org", 18333);
        } else if(_wallet->network() == Coin::Network::mainnet) {
            _wallet->sync("seed.bitcoin.sipa.be", 8333);
        }else{
            //this shouldn't throw.. its called from the timer!
            return;
        }
    } else {
        _wallet->sync(_settings.bitcoinNodeHost, _settings.bitcoinNodePort);
    }
}

void AppKit::shutdown(const Callback & shutdownComplete) {
    if(_shuttingDown)
        return;

    std::cout << "Shutting down AppKit" << std::endl;

    _shuttingDown = true;

    _timer->stop();

    _node->pause([shutdownComplete, this](){
        std::cout << "Node paused" << std::endl;

        std::cout << "Stopping wallet" << std::endl;
        _wallet->stopSync();

        _dataDirectory->unlock();

        std::cout << "AppKit shutdown complete" << std::endl;
        shutdownComplete();
    });
}

void AppKit::applySettings(const Settings & settings) {
    if(_settings.bitcoinNodeHost != settings.bitcoinNodeHost || _settings.bitcoinNodePort != settings.bitcoinNodePort)
        _wallet->stopSync();

    _settings = settings;

    syncWallet();
}

SavedTorrents AppKit::generateSavedTorrents() const {
    return SavedTorrents(_node.get());
}

void AppKit::addTorrent(const core::TorrentIdentifier &torrentReference, const core::Node::AddedTorrent &addedTorrent){
        _node->addTorrent(0,0,
                         libtorrent::to_hex(torrentReference.infoHash().to_string()),
                         std::vector<char>(),
                          _dataDirectory->defaultSavePath().toStdString(),
                         false,
                         torrentReference,
                         addedTorrent);
}

void AppKit::addTorrent(const SavedTorrentParameters &torrent, const core::Node::AddedTorrent &addedTorrent) {

    _node->addTorrent(torrent.uploadLimit(),
                      torrent.downloadLimit(),
                      torrent.name(),
                      torrent.resumeData(),
                      torrent.savePath(),
                      torrent.paused(),
                      torrent.metaData(),
                      addedTorrent);
}

void AppKit::buyTorrent(const core::Torrent *torrent,
                        const protocol_session::BuyingPolicy& policy,
                        const protocol_wire::BuyerTerms& terms,
                        const extension::request::SubroutineHandler& handler){

    if(libtorrent::torrent_status::state_t::downloading != torrent->state()) {
        throw std::runtime_error("torrent must be in downloading state to buy");
    }

    auto contractFunds = estimateRequiredFundsToBuyTorrent(torrent, terms);

    auto outputs = _wallet->lockOutputs(contractFunds, 0);

    if(outputs.size() == 0) {
        // Not enough funds
        std::cout << "Not Enough Funds" << std::endl;
        return;
    }

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    buyTorrent(plugin, policy, terms, handler, outputs);
}

void AppKit::buyTorrent(core::TorrentPlugin *plugin,
                        const protocol_session::BuyingPolicy& policy,
                        const protocol_wire::BuyerTerms& terms,
                        const extension::request::SubroutineHandler& handler,
                        Coin::UnspentOutputSet outputs){

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

    if(libtorrent::torrent_status::state_t::seeding != torrent->state()) {
        throw std::runtime_error("torrent must be in seeding state to sell");
    }

    core::TorrentPlugin* plugin = torrent->torrentPlugin();

    sellTorrent(plugin, policy, terms, handler);
}

uint64_t AppKit::estimateRequiredFundsToBuyTorrent(const core::Torrent *torrent, joystream::protocol_wire::BuyerTerms terms) {

    auto metadata = torrent->metaData().lock();
    uint64_t paymentChannelFunds = metadata->num_pieces() * terms.maxPrice();

    // there is a circular condition here, we are trying to estimate the amount of value to lock and for that we need to know
    // how many utxo will be locked which cannot be determined before locking the amount.. so we will just assume a single utxo
    // will be locked, this may likely result in the fee being too low if we end up locking significantly more utxos
    // This one more problem which will be solved when we address the prefunding mechanism
    // https://github.com/JoyStream/JoyStream/issues/315
    uint64_t estimatedContractFee = paymentchannel::Contract::fee(terms.minNumberOfSellers(), true, terms.maxContractFeePerKb(), 1);

    return paymentChannelFunds + estimatedContractFee;
}

} // appkit namespace
} // joystream namespace
