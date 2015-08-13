/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <core/extension/Alert/StartedBuyerTorrentPlugin.hpp>

StartedBuyerTorrentPlugin::StartedBuyerTorrentPlugin() {
}

StartedBuyerTorrentPlugin::StartedBuyerTorrentPlugin(const libtorrent::sha1_hash & infoHash,
                                                     const BuyerTorrentPlugin::Configuration & configuration,
                                                     const Coin::UnspentP2PKHOutput & utxo,
                                                     const BuyerTorrentPlugin::Status & status)
    : _infoHash(infoHash)
    , _configuration(configuration)
    , _utxo(utxo)
    , _status(status) {
}

StartedBuyerTorrentPlugin::StartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin & alert)
    : _infoHash(alert.infoHash())
    , _configuration(alert.configuration())
    , _utxo(alert.utxo())
    , _status(alert.status()){
}

int StartedBuyerTorrentPlugin::type() const {
    return alert_type;
}

char const* StartedBuyerTorrentPlugin::what() const {
    return "StartedBuyerTorrentPlugin";
}

std::string StartedBuyerTorrentPlugin::message() const {
    return std::string("StartedBuyerTorrentPlugin::message: IMPLEMENT LATER");
}

int StartedBuyerTorrentPlugin::category() const {
    return libtorrent::alert::status_notification;
}

std::auto_ptr<libtorrent::alert> StartedBuyerTorrentPlugin::clone() const {
    return std::auto_ptr<libtorrent::alert>(new StartedBuyerTorrentPlugin(*this));
}

libtorrent::sha1_hash StartedBuyerTorrentPlugin::infoHash() const {
    return _infoHash;
}

void StartedBuyerTorrentPlugin::setInfoHash(const libtorrent::sha1_hash & infoHash) {
    _infoHash = infoHash;
}

BuyerTorrentPlugin::Configuration StartedBuyerTorrentPlugin::configuration() const {
    return _configuration;
}

void StartedBuyerTorrentPlugin::setConfiguration(const BuyerTorrentPlugin::Configuration & configuration) {
    _configuration = configuration;
}

Coin::UnspentP2PKHOutput StartedBuyerTorrentPlugin::utxo() const {
    return _utxo;
}

void StartedBuyerTorrentPlugin::setUtxo(const Coin::UnspentP2PKHOutput &utxo) {
    _utxo = utxo;
}

BuyerTorrentPlugin::Status StartedBuyerTorrentPlugin::status() const {
    return _status;
}

void StartedBuyerTorrentPlugin::setStatus(const BuyerTorrentPlugin::Status &status) {
    _status = status;
}

