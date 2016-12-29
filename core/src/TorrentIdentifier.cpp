/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 21 2016
 */

#include <core/TorrentIdentifier.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

namespace joystream {
namespace core {

TorrentIdentifier::TorrentIdentifier(const libtorrent::sha1_hash & infoHash)
    : _type(Type::Hash)
    , _infoHash(infoHash) {
}

TorrentIdentifier::TorrentIdentifier(const MagnetLink & magnetLink)
    : _type(Type::MagnetLink)
    , _magnetLink(magnetLink)
    , _infoHash(magnetLink.infoHash()) {
}

TorrentIdentifier::TorrentIdentifier(const boost::shared_ptr<libtorrent::torrent_info> & torrentFile)
    : _type(Type::TorrentFile)
    , _torrentFile(torrentFile)
    , _infoHash(torrentFile->info_hash()) {
}

TorrentIdentifier::TorrentIdentifier(const TorrentIdentifier* ti)
    : _type(ti->type())
    , _torrentFile(ti->torrentFile())
    , _infoHash(ti->infoHash()) {
}

TorrentIdentifier TorrentIdentifier::fromTorrentFilePath(const std::string &path)
{
    libtorrent::error_code ec;
    boost::shared_ptr<libtorrent::torrent_info> ti;

    if(!boost::filesystem::exists(path))
        throw std::runtime_error("path does not exist");

    if(!boost::filesystem::is_regular_file(path))
        throw std::runtime_error("path is not a regular file");

    ti = boost::make_shared<libtorrent::torrent_info>(path, boost::ref(ec), 0);

    if (ec)
        throw std::runtime_error(ec.message());

    return joystream::core::TorrentIdentifier(ti);

}

TorrentIdentifier TorrentIdentifier::fromTorrentFileContents(const std::vector<unsigned char> &torrentFileData) {
    libtorrent::error_code ec;
    boost::shared_ptr<libtorrent::torrent_info> ti;

    ti = boost::make_shared<libtorrent::torrent_info>((const char*)torrentFileData.data(), torrentFileData.size(), boost::ref(ec), 0);

    if (ec)
        throw std::runtime_error(ec.message());

    return joystream::core::TorrentIdentifier(ti);
}

TorrentIdentifier TorrentIdentifier::fromHashString(const std::string &hexHashString)
{

    if(hexHashString.size() != 40)
        throw std::runtime_error("incorrent length of hex string");

    char buf[21];

    if(!libtorrent::from_hex(hexHashString.c_str(), hexHashString.size(), buf))
        throw std::runtime_error("invalid hex string");

    return joystream::core::TorrentIdentifier(libtorrent::sha1_hash(buf));
}

TorrentIdentifier TorrentIdentifier::fromMagnetLinkString(const std::string &uri)
{
    auto magnetLink = joystream::core::MagnetLink::fromURI(uri);
    return joystream::core::TorrentIdentifier(magnetLink);
}

TorrentIdentifier::Type TorrentIdentifier::type() const noexcept {
    return _type;
}

libtorrent::sha1_hash TorrentIdentifier::infoHash() const noexcept {
    return _infoHash;
}

MagnetLink TorrentIdentifier::magnetLink() const noexcept {
    return _magnetLink;
}

boost::shared_ptr<libtorrent::torrent_info> TorrentIdentifier::torrentFile() const noexcept {
    return _torrentFile;
}

}
}
