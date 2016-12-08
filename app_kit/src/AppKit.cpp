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
#include <app_kit/TorrentObserver.hpp>

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

std::shared_ptr<WorkerResult> AppKit::addTorrent(const SavedTorrentParameters &torrent) {

    auto metadata = torrent.metaData();

    AddTorrentRequest request(metadata && metadata->is_valid() ? metadata : core::TorrentIdentifier(torrent.infoHash()),
                              torrent.savePath());

    request.uploadLimit = torrent.uploadLimit();
    request.downloadLimit = torrent.downloadLimit();
    request.name = torrent.name();
    request.resumeData = torrent.resumeData();
    request.paused = torrent.paused();

    return addTorrent(request);
}

std::shared_ptr<WorkerResult> AppKit::addTorrent(const core::TorrentIdentifier & ti, const std::string& savePath) {

    AddTorrentRequest request(ti, savePath);

    return addTorrent(request);
}

std::shared_ptr<WorkerResult> AppKit::addTorrent(AddTorrentRequest& request) {

    return TorrentAdder::add(this, node(), request);
}

std::shared_ptr<WorkerResult> AppKit::buyTorrent(libtorrent::sha1_hash infoHash,
                                                 const protocol_session::BuyingPolicy& policy,
                                                 const protocol_wire::BuyerTerms& terms,
                                                 protocol_session::SessionState state) {
    return TorrentBuyer::buy(this, node(), wallet(), infoHash, policy, terms, state, paychanKeysGenerator(), receiveAddressesGenerator(), changeAddressesGenerator());
}

std::shared_ptr<WorkerResult> AppKit::sellTorrent(libtorrent::sha1_hash infoHash,
                                                  const protocol_session::SellingPolicy& policy,
                                                  const protocol_wire::SellerTerms& terms,
                                                  protocol_session::SessionState state) {
    return TorrentSeller::sell(this, node(), wallet(), infoHash, policy, terms, state, paychanKeysGenerator(), receiveAddressesGenerator());
}

std::shared_ptr<WorkerResult> AppKit::observeTorrent(libtorrent::sha1_hash infoHash, protocol_session::SessionState state) {
    return TorrentObserver::observe(this, node(), infoHash, state);
}

void AppKit::broadcastTransaction(Coin::Transaction &tx) const {
    _transactionSendBuffer->insert(tx);
}

protocol_session::GenerateP2SHKeyPairCallbackHandler AppKit::paychanKeysGenerator() {
    return [this](const protocol_session::P2SHScriptGeneratorFromPubKey& generateScript, const uchar_vector& data) -> Coin::KeyPair {
        return paychanKeysGeneratorFunction(generateScript, data);
    };
}

Coin::KeyPair AppKit::paychanKeysGeneratorFunction(const protocol_session::P2SHScriptGeneratorFromPubKey& generateScript, const uchar_vector& data)
{
    Coin::PrivateKey sk = _wallet->generateKey([&generateScript, &data](const Coin::PublicKey & pk){
        return bitcoin::RedeemScriptInfo(generateScript(pk), data);
    });

    return Coin::KeyPair(sk);
}

protocol_session::GenerateReceiveAddressesCallbackHandler AppKit::receiveAddressesGenerator() {
    return [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
        return receiveAddressesGeneratorFunction(npairs);
    };
}

std::vector<Coin::P2PKHAddress> AppKit::receiveAddressesGeneratorFunction(int npairs) {
    std::vector<Coin::P2PKHAddress> addresses;

    for(int n = 0; n < npairs; n++) {
        addresses.push_back(_wallet->generateReceiveAddress());
    }

    return addresses;
}

protocol_session::GenerateChangeAddressesCallbackHandler AppKit::changeAddressesGenerator() {
    return [this](int npairs) -> std::vector<Coin::P2PKHAddress> {
        return changeAddressesGeneratorFunction(npairs);
    };
}

std::vector<Coin::P2PKHAddress> AppKit::changeAddressesGeneratorFunction(int npairs) {
    std::vector<Coin::P2PKHAddress> addresses;

    for(int n = 0; n < npairs; n++) {
        addresses.push_back(_wallet->generateChangeAddress());
    }

    return addresses;
}

} // appkit namespace
} // joystream namespace
