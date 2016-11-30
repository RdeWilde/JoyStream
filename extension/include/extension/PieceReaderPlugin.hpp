/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 27 2016
 */

#ifndef JOYSTREAM_EXTENSION_PIECE_READER_PLUGIN_HPP
#define JOYSTREAM_EXTENSION_PIECE_READER_PLUGIN_HPP

#include <extension/PieceReaderInterface.hpp>
#include <libtorrent/extensions.hpp>
#include <common/common.hpp>

#include <unordered_map>

namespace libtorrent {
    struct read_piece_alert;
}

namespace joystream {
namespace extension {
namespace detail {

    struct Torrent {

        Torrent(const libtorrent::torrent_handle & torrentHandle)
            : _torrentHandle(torrentHandle) {}

        typedef std::set<AbstractPieceReadHandler *> HandlerSet;

        libtorrent::torrent_handle _torrentHandle;
        std::unordered_map<int, HandlerSet> _pieceIndexToRegisteredHandlers;
    };

    //typedef std::unordered_map<libtorrent::sha1_hash, Torrent> TorrentToPieceHandlers;
}

class PieceReaderPlugin;

/**
 * @brief Reads pieces from libtorrent session.
 */
class PieceReader : public PieceReaderInterface {

public:

    virtual int readPiece(int pieceIndex, AbstractPieceReadHandler * handler);

    virtual int cancelRequests(AbstractPieceReadHandler * handler);

private:

    libtorrent::session_handle h;

    std::unordered_map<libtorrent::sha1_hash, detail::Torrent> _torrents;

    // PieceReaderPlugin is the only class allowed to construct
    // objects of this class by invoking the private constructor,
    // or make calls about the results of piece reading requests
    // to libtorrent.
    friend class PieceReaderPlugin;

    PieceReader(libtorrent::session_handle h);

    /// libtorrent::plugin events

    void pieceRead(const libtorrent::sha1_hash & infoHash, int pieceIndex, int size, const boost::shared_array<char> & buffer);

    void pieceReadFailed(const libtorrent::sha1_hash & infoHash, int pieceIndex, const libtorrent::error_code & ec);

    // Return, and remove, the set of handlers on given torrent and piece
    detail::HandlerSet extractHandlerSet(const libtorrent::sha1_hash & infoHash, int pieceIndex);

    void askLibtorrentTorReadPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex);

};

/**
 * @brief A plugin which reads pieces
 */
class PieceReaderPlugin : public libtorrent::plugin {

public:

    virtual void added(libtorrent::session_handle h);

    virtual void on_alert(const libtorrent::alert *);

    const boost::optional<PieceReader> & pieceReader() const noexcept;

private:

    boost::optional<PieceReader> _pieceReader;
};

}
}

#endif // JOYSTREAM_EXTENSION_PIECE_READER_PLUGIN_HPP
