/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 2016
 */

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
#include <QJsonArray>

#include <QDir>

namespace joystream {
namespace appkit {

bitcoin::SPVWallet * AppKit::getWallet(const std::string &storeFile, const std::string blockTreeFile, Coin::Network network) {

    auto wallet = new bitcoin::SPVWallet(storeFile, blockTreeFile, network);

    std::cout << "Looking for wallet file " << storeFile << std::endl;

    if(!QFile::exists(QString::fromStdString(storeFile))){
        std::cout << "Wallet not found.\nCreating a new wallet..." << std::endl;
        wallet->create();
    } else {
        std::cout << "Wallet found, opening..." << std::endl;
        wallet->open();
    }

    return wallet;
}

AppKit* AppKit::create(const std::string &walletFilePath,
                       const std::string &walletBlockTreeFilePath,
                       Coin::Network network, const Settings &settings)
{

    bitcoin::SPVWallet* wallet = nullptr;
    core::Node* node = nullptr;
    TransactionSendBuffer *txSendBuffer = nullptr;

    wallet = getWallet(walletFilePath, walletBlockTreeFilePath, network);

    if(settings.autoStartWalletSync)
        wallet->loadBlockTree();

    txSendBuffer = new TransactionSendBuffer(wallet);

    node = core::Node::create([txSendBuffer](const Coin::Transaction &tx){
        txSendBuffer->insert(tx);
    });

    return new AppKit(node, wallet, txSendBuffer, settings);
}

AppKit::AppKit(core::Node* node,
               bitcoin::SPVWallet* wallet,
               TransactionSendBuffer *txSendBuffer,
               const Settings &settings)
    : _node(node),
      _wallet(wallet),
      _transactionSendBuffer(txSendBuffer),
      _settings(settings),
      _trySyncWallet(settings.autoStartWalletSync) {

    QObject::connect(&_timer, &QTimer::timeout, [this](){
        _node->updateStatus();

        // Try to reconnect to bitcoin network if wallet went offline
        if(_trySyncWallet)
            syncWallet();

        // Sendout queued transactions
        _transactionSendBuffer->flush();
    });

    _timer.start(5000);
}

core::Node *AppKit::node() {
    return _node.get();
}

bitcoin::SPVWallet* AppKit::wallet() {
    return _wallet.get();
}

void AppKit::syncWallet() {
    _trySyncWallet = true;

    if(_settings.bitcoinNodeHost.empty()) {
        if(_wallet->network() == Coin::Network::testnet3) {
            _wallet->sync("testnet-seed.bitcoin.petertodd.org", 18333);
        } else if(_wallet->network() == Coin::Network::mainnet) {
            _wallet->sync("seed.bitcoin.sipa.be", 8333);
        }
    } else {
        _wallet->sync(_settings.bitcoinNodeHost, _settings.bitcoinNodePort);
    }
}

void AppKit::shutdown(const Callback & shutdownComplete) {   
    std::cout << "Shutting down AppKit" << std::endl;

    _timer.stop();

    _node->pause([shutdownComplete, this](){
        std::cout << "Node paused" << std::endl;

        std::cout << "Stopping wallet" << std::endl;
        _wallet->stopSync();

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
    return SavedTorrents(_node->torrents());
}

void AppKit::addTorrent(const SavedTorrentParameters &torrent, const core::Node::AddedTorrent &addedTorrent) {
    auto metadata = torrent.metaData();

    _node->addTorrent(torrent.uploadLimit(),
                      torrent.downloadLimit(),
                      torrent.name(),
                      torrent.resumeData(),
                      torrent.savePath(),
                      torrent.paused(),
                      metadata && metadata->is_valid() ? metadata : core::TorrentIdentifier(torrent.infoHash()),
                      addedTorrent);
}

void AppKit::buyTorrent(int64_t contractFundingAmount,
                        const core::Torrent *torrent,
                        const protocol_session::BuyingPolicy& policy,
                        const protocol_wire::BuyerTerms& terms,
                        const extension::request::SubroutineHandler& handler){

    if(libtorrent::torrent_status::state_t::downloading != torrent->state()) {
        throw std::runtime_error("torrent must be in downloading state to buy");
    }

    auto outputs = _wallet->lockOutputs(contractFundingAmount, 0);

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

void AppKit::broadcastTransaction(Coin::Transaction &tx) const {
    _transactionSendBuffer->insert(tx);
}

} // appkit namespace
} // joystream namespace
