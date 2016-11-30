/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 27 2016
 */

#include <extension/PieceReaderPlugin.hpp>
#include <libtorrent/alert_types.hpp>

namespace joystream {
namespace extension {

/// PieceReader

PieceReader::PieceReader(libtorrent::session_handle h)
    : h(h) {}

int PieceReader::readPiece(int pieceIndex, AbstractPieceReadHandler * handler) {

    auto torrentToPieceHandlersIterator = _torrentToPieceHandlers.find(handler->infoHash());

    if(torrentToPieceHandlersIterator == _torrentToPieceHandlers.end()) {

        _torrentToPieceHandlers.insert(std::make_pair(handler->infoHash(), detail::PieceIndexToHandlers({{pieceIndex, detail::HandlerSet({handler})}})));

        return 1;

    } else {

        detail::PieceIndexToHandlers & pieceIndexToHandlers = torrentToPieceHandlersIterator->second;

        auto pieceIndexToHandlersIterator = pieceIndexToHandlers.find(pieceIndex);

        if(pieceIndexToHandlersIterator == pieceIndexToHandlers.end()) {

            pieceIndexToHandlers.insert(std::make_pair(pieceIndex, detail::HandlerSet({handler})));

            return 1;

        } else {

            detail::HandlerSet & handlerSet = pieceIndexToHandlersIterator->second;

            // only non-empty sets should exist
            assert(!handlerSet.empty());

            // Remember old size to see if we are able to inser handler
            auto oldSize = handlerSet.size();

            // Try to insert handler
            handlerSet.insert(handler);

            // If set grew, then it handler was new, and we are done
            if(handlerSet.size() == oldSize + 1)
                return handlerSet.size();
            else
                throw exception::HandlerAlreadyRegisteredWithPiece(pieceIndex, handler);
        }

    }
}

int PieceReader::cancelRequests(AbstractPieceReadHandler * handler) {

    // Try to lookup what handlers are registered on the torrent to which this handler corresponds
    auto torrentToPieceHandlersIterator = _torrentToPieceHandlers.find(handler->infoHash());

    if(torrentToPieceHandlersIterator != _torrentToPieceHandlers.end()) {

        // Iterate all pieces, on the given torrent, for which there are handlers registered
        detail::PieceIndexToHandlers & pieceIndexToHandlers = torrentToPieceHandlersIterator->second;

        // Number of pieces for which handler was registered
        int numberOfPiecesHandlerWasRegisteredWith = 0;

        for(auto it = pieceIndexToHandlers.begin(); it != pieceIndexToHandlers.end();) {

            detail::HandlerSet & handlerSet = it->second;

            // only non-empty sets should exist
            assert(!handlerSet.empty());

            // Check if the provided handler is registered with this piece
            auto handlerSetIterator = handlerSet.find(handler);

            if(handlerSetIterator != handlerSet.end()) {

                // if so, then remove it
                handlerSet.erase(handlerSetIterator);

                // and count removal
                numberOfPiecesHandlerWasRegisteredWith++;

                // if that was last handler in set,
                if(handlerSet.empty())
                    // then drop set
                    it = pieceIndexToHandlers.erase(it); // erase iterator over which we are iterating
            } else
                ++it;

        }

        // if there are no pieces left
        if(pieceIndexToHandlers.empty())
            _torrentToPieceHandlers.erase(torrentToPieceHandlersIterator);


        return numberOfPiecesHandlerWasRegisteredWith;
    }

    return 0;
}

void PieceReader::pieceRead(const libtorrent::sha1_hash & infoHash, int pieceIndex, int size, const boost::shared_array<char> & buffer) {

    detail::HandlerSet set = extractHandlerSet(infoHash,pieceIndex);

    for(AbstractPieceReadHandler * handler : set)
        handler->pieceReadCompleted(pieceIndex, size, buffer);
}

void PieceReader::pieceReadFailed(const libtorrent::sha1_hash & infoHash, int pieceIndex, const libtorrent::error_code & ec) {

    detail::HandlerSet set = extractHandlerSet(infoHash,pieceIndex);

    for(AbstractPieceReadHandler * handler : set)
        handler->pieceReadFailed(pieceIndex, ec);
}

detail::HandlerSet PieceReader::extractHandlerSet(const libtorrent::sha1_hash & infoHash, int pieceIndex) {

    auto torrentToPieceHandlersIterator = _torrentToPieceHandlers.find(infoHash);

    if(torrentToPieceHandlersIterator != _torrentToPieceHandlers.end()) {

        detail::PieceIndexToHandlers & pieceIndexToHandlers = torrentToPieceHandlersIterator->second;

        auto handlerSetIterator = pieceIndexToHandlers.find(pieceIndex);

        if(handlerSetIterator != pieceIndexToHandlers.end()) {

            // COPY handler set
            detail::HandlerSet handlerSet = handlerSetIterator->second;

            // only non-empty sets should exist
            assert(!handlerSet.empty());

            // Erase original
            pieceIndexToHandlers.erase(handlerSetIterator);

            // Return COPY
            return handlerSet;
        }

    }

    return detail::HandlerSet();
}

/// PieceReaderPlugin

void PieceReaderPlugin::added(libtorrent::session_handle h) {

    assert(!_pieceReader.is_initialized());

    _pieceReader = PieceReader(h);

    std::clog << "PieceReaderPlugin added to session." << std::endl;
}

void PieceReaderPlugin::on_alert(const libtorrent::alert * a) {

    assert(_pieceReader.is_initialized());

    if(const libtorrent::read_piece_alert * p = libtorrent::alert_cast<libtorrent::read_piece_alert>(a)) {

        if(p->ec)
           _pieceReader->pieceReadFailed(p->handle.info_hash(), p->piece, p->ec);
        else
           _pieceReader->pieceRead(p->handle.info_hash(), p->piece, p->size, p->buffer);
    }

}

const boost::optional<PieceReader> & PieceReaderPlugin::pieceReader() const noexcept {
    return _pieceReader;
}

}
}
