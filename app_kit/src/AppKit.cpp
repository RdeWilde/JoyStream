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
#include <app_kit/AddTorrentRequest.hpp>
#include <app_kit/TorrentAdder.hpp>
#include <app_kit/TorrentBuyer.hpp>
#include <app_kit/TorrentSeller.hpp>

#include <core/core.hpp>
#include <bitcoin/SPVWallet.hpp>

#include <libtorrent/create_torrent.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QDir>
#include <memory>

namespace joystream {
namespace appkit {

AppKit* AppKit::create(const std::string &walletFilePath,
                       const std::string &walletBlockTreeFilePath,
                       Coin::Network network, const Settings &settings)
{
    std::unique_ptr<bitcoin::SPVWallet> wallet(new bitcoin::SPVWallet(walletFilePath, walletBlockTreeFilePath, network));

    if(settings.autoStartWalletSync)
        wallet->loadBlockTree();

    std::unique_ptr<TransactionSendBuffer> txSendBuffer(new TransactionSendBuffer(wallet.get()));

    auto txSendBufferPtr = txSendBuffer.get();
    std::unique_ptr<core::Node> node(core::Node::create([txSendBufferPtr](const Coin::Transaction &tx){
        txSendBufferPtr->insert(tx);
    }));

    return new AppKit(settings, wallet, txSendBuffer, node);
}

AppKit::AppKit(const Settings &settings,
               std::unique_ptr<bitcoin::SPVWallet>& wallet,
               std::unique_ptr<TransactionSendBuffer>& txSendBuffer,
               std::unique_ptr<core::Node>& node)
    : _settings(settings),
      _trySyncWallet(settings.autoStartWalletSync),
      _wallet(std::move(wallet)),
      _transactionSendBuffer(std::move(txSendBuffer)),
      _node(std::move(node)) {

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

std::shared_ptr<AddTorrentResponse> AppKit::addTorrent(const SavedTorrentParameters &torrent) {

    auto metadata = torrent.metaData();

    AddTorrentRequest request(metadata && metadata->is_valid() ? metadata : core::TorrentIdentifier(torrent.infoHash()),
                              torrent.savePath());

    request.uploadLimit = torrent.uploadLimit();
    request.downloadLimit = torrent.downloadLimit();
    request.name = torrent.name() == "" ? libtorrent::to_hex(request.torrentIdentifier.infoHash().to_string()) : torrent.name();
    request.resumeData = torrent.resumeData();
    request.paused = torrent.paused();

    return TorrentAdder::add(this, node(), request);
}

std::shared_ptr<AddTorrentResponse> AppKit::addTorrent(const core::TorrentIdentifier & ti, const std::string& savePath) {

    AddTorrentRequest request(ti, savePath);

    return TorrentAdder::add(this, node(), request);
}

std::shared_ptr<BuyTorrentResponse> AppKit::buyTorrent(libtorrent::sha1_hash infoHash,
                                                       const protocol_session::BuyingPolicy& policy,
                                                       const protocol_wire::BuyerTerms& terms) {
    return TorrentBuyer::buy(this, node(), wallet(), infoHash, policy, terms);
}

std::shared_ptr<SellTorrentResponse> AppKit::sellTorrent(libtorrent::sha1_hash infoHash,
                                                         const protocol_session::SellingPolicy& policy,
                                                         const protocol_wire::SellerTerms& terms) {
    return TorrentSeller::sell(this, node(), wallet(), infoHash, policy, terms);
}

void AppKit::broadcastTransaction(Coin::Transaction &tx) const {
    _transactionSendBuffer->insert(tx);
}

} // appkit namespace
} // joystream namespace
