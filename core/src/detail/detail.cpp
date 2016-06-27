/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#include <core/detail/detail.hpp>
#include <core/Node.hpp>
#include <extension/extension.hpp>

namespace joystream {
namespace core {
namespace detail {

Torrent::Torrent(extension::Plugin * plugin,
                 const libtorrent::sha1_hash & infoHash,
                 const std::string & name,
                 const std::string & savePath,
                 const std::vector<char> & resumeData,
                 std::uint64_t flags,
                 State event)
    : plugin(plugin)
    , infoHash(infoHash)
    , name(name)
    , savePath(savePath)
    , resumeData(resumeData)
    , flags(flags)
    , state(event)
    /**
    , _model(infoHash,
             name,
             savePath,
             torrentFile) {
                 */
{
}

void Torrent::start(const extension::request::Start::ResultHandler & handler) {
    plugin->submit(extension::request::Start(infoHash, handler));
}

void Torrent::stop(const extension::request::Stop::ResultHandler & handler) {
    plugin->submit(extension::request::Stop(infoHash, handler));
}

void Torrent::pause(const extension::request::Pause::ResultHandler & handler) {
   plugin->submit(extension::request::Pause(infoHash, handler));
}

void Torrent::updateTerms(const protocol_wire::BuyerTerms & terms, const extension::request::UpdateBuyerTerms::ResultHandler & handler) {
    plugin->submit(extension::request::UpdateBuyerTerms(infoHash, terms, handler));
}

void Torrent::updateTerms(const protocol_wire::SellerTerms & terms, const extension::request::UpdateSellerTerms::ResultHandler & handler) {
    plugin->submit(extension::request::UpdateSellerTerms(infoHash, terms, handler));
}

void Torrent::toObserveMode(const extension::request::ToObserveMode::ResultHandler & handler) {
    plugin->submit(extension::request::ToObserveMode(infoHash, handler));
}

void Torrent::toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                         const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                         const protocol_session::SellingPolicy & sellingPolicy,
                         const protocol_wire::SellerTerms & terms,
                         const extension::request::ToSellMode::ResultHandler & handler) {

    plugin->submit(extension::request::ToSellMode(infoHash,
                                                   generateKeyPairsCallbackHandler,
                                                   generateP2PKHAddressesCallbackHandler,
                                                   sellingPolicy,
                                                   terms,
                                                   handler));
}

void Torrent::toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                        const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                        const Coin::UnspentP2PKHOutput & funding,
                        const protocol_session::BuyingPolicy & policy,
                        const protocol_wire::BuyerTerms & terms,
                        const extension::request::ToBuyMode::ResultHandler & handler) {

    plugin->submit(extension::request::ToBuyMode(infoHash,
                                                  generateKeyPairsCallbackHandler,
                                                  generateP2PKHAddressesCallbackHandler,
                                                  funding,
                                                  policy,
                                                  terms,
                                                  handler));
}

/**
void Torrent::addPlugin(const SellerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Seller;
    _model.addPlugin(status);
}

void Torrent::addPlugin(const BuyerTorrentPlugin::Status & status) {

    Q_ASSERT(_pluginInstalled == PluginInstalled::None);
    _pluginInstalled = PluginInstalled::Buyer;
    _model.addPlugin(status);
}
*/

/**
libtorrent::sha1_hash Torrent::infoHash() const {
    return infoHash;
}

std::string Torrent::name() const {
    return name;
}

void Torrent::setName(const std::string & name) {
    name = name;
}

std::string Torrent::savePath() const {
    return savePath;
}

void Torrent::setSavePath(const std::string & savePath) {
    savePath = savePath;
}

std::vector<char> Torrent::resumeData() const {
    return resumeData;
}

void Torrent::setResumeData(const std::vector<char> & resumeData) {
    resumeData = resumeData;
}


std::uint64_t Torrent::flags() const {
    return flags;
}

void Torrent::setFlags(std::uint64_t flags) {
    flags = flags;
}

libtorrent::torrent_info * Torrent::torrentInfo() {
    return _torrentInfo;
}

libtorrent::torrent_handle Torrent::handle() const {
    return _handle;
}

void Torrent::setHandle(const libtorrent::torrent_handle & handle) {
    _handle = handle;
}

Torrent::State Torrent::state() const {
    return state;
}

void Torrent::setState(State event) {
    state = event;
}

std::shared_ptr<core::Torrent> Torrent::model() const {
    return model;
}

void Torrent::addStream(Stream * stream) {
    _streams.insert(stream);
}

void Torrent::removeStream(Stream * stream) {
    _streams.remove(stream);
}

void Torrent::pieceRead(const boost::shared_array<char> & buffer,
                                    int pieceIndex,
                                    int size) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceRead(buffer, pieceIndex, size);
}

void Torrent::pieceFinished(int piece) {

    // Iterate streams and notify them
    for(QSet<Stream *>::iterator i = _streams.begin(),
        end = _streams.end();
        i != end;i++)
        (*i)->pieceDownloaded(piece);
}
*/

}
}
}
