/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#include <core/detail/Torrent.hpp>

namespace joystream {
namespace core {
namespace detail {

Torrent::Torrent() {}

Torrent::Torrent(const libtorrent::sha1_hash & infoHash,
                             const std::string & name,
                             const std::string & savePath,
                             const std::vector<char> & resumeData,
                             std::uint64_t flags,
                             const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                             State event)
    : _infoHash(infoHash)
    , _name(name)
    , _savePath(savePath)
    , _resumeData(resumeData)
    , _flags(flags)
    , _status(event)
    , _model(infoHash,
             name,
             savePath,
             torrentFile) {
}

/**
>>>>>>> 4ef7f8401ad7b8e5ab4238fb2df82adcb2a5d06f
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

libtorrent::sha1_hash Torrent::infoHash() const {
    return _infoHash;
}

std::string Torrent::name() const {
    return _name;
}

void Torrent::setName(const std::string & name) {
    _name = name;
}

std::string Torrent::savePath() const {
    return _savePath;
}

void Torrent::setSavePath(const std::string & savePath) {
    _savePath = savePath;
}

std::vector<char> Torrent::resumeData() const {
    return _resumeData;
}

void Torrent::setResumeData(const std::vector<char> & resumeData) {
    _resumeData = resumeData;
}


std::uint64_t Torrent::flags() const {
    return _flags;
}

void Torrent::setFlags(std::uint64_t flags) {
    _flags = flags;
}

/**
libtorrent::torrent_info * Torrent::torrentInfo() {
    return _torrentInfo;
}
*/

/**
libtorrent::torrent_handle Torrent::handle() const {
    return _handle;
}

void Torrent::setHandle(const libtorrent::torrent_handle & handle) {
    _handle = handle;
}
*/

Torrent::State Torrent::state() const {
    return _status;
}

void Torrent::setState(State event) {
    _status = event;
}

std::weak_ptr<core::Torrent> Torrent::model() {
    return _model;

/**
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
