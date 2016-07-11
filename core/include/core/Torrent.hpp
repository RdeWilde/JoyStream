/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 17 2016
 */

#ifndef JOYSTREAM_CORE_TORRENT_HPP
#define JOYSTREAM_CORE_TORRENT_HPP

#include <extension/extension.hpp>
#include <core/Node.hpp>
#include <core/Configuration.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <memory>

namespace joystream {
namespace core {

class Peer;
class TorrentPlugin;

class Torrent : public QObject {

    Q_OBJECT

private:

    Torrent(const libtorrent::sha1_hash &);

public:

    // No public constructor

    // All user operations (method requiring talking to anything outside of this object, i.e. not simple getters):
    // ---------------------
    // 1) for all operations, try to lock underyling detail::torrent
    // client first, throw exception if its not possible or if its
    // possible, but currently in state "being_removed".
    // 2) if its possible, and its in state "being_added", then assert
    // failure, as no reference should ever have been given out by node, nor
    // should node have called that operation
    // 3) <what to do about Node state?>: jUST THROW IT ITS NOT  STARTED?

    // Allow checking liveness without
    // add bool expired() const noexcept;

    /**
    // Stream management
    void addStream(Stream * stream);
    void removeStream(Stream * stream);

    // Given piece was read
    void pieceRead(const boost::shared_array<char> & buffer, int pieceIndex, int size);

    // Given piece was downloaded and checked
    void pieceFinished(int piece);
    */

    libtorrent::sha1_hash infoHash() const noexcept;


    // streaming routines
    // this can be used to stream into a http daemon, etc.
    // *initiating stream (register a callback)
    // *stopping stream,


    // All streams for this torrent.
    // Not quite sure if multiple separate streams for one torrent
    // is necessary, if not, then remove this QSet later.
    //QSet<Stream *> _streams;

signals:

    // Classic vanilla libtorrent torrent state changes

private:

    friend class Node;

    // Info hash
    libtorrent::sha1_hash _infoHash;

    // Handle to torrent
    // A valid handle is only set after the torrent has been added
    // successfully to session
    libtorrent::torrent_handle _handle;

    // Peers
    std::map<libtorrent::tcp::endpoint, std::shared_ptr<Peer>> _peers;

    // TorrentPlugin
    std::shared_ptr<TorrentPlugin> _torrentPlugin;

    /// User provided callback state

    // Node::addTorrent
    boost::optional<detail::AddTorrentCall> _addTorrentCall;

    // Node::removeTorrent


};

}
}

#endif // JOYSTREAM_CORE_TORRENT_HPP
