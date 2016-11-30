/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 27 2016
 */

#ifndef JOYSTREAM_EXTENSION_PIECE_READER_PLUGIN_INTERFACE_HPP
#define JOYSTREAM_EXTENSION_PIECE_READER_PLUGIN_INTERFACE_HPP

#include <libtorrent/sha1_hash.hpp>

#include <set>

namespace boost {
namespace system {
    class error_code;
}
    template<class T> class shared_array;
}

namespace libtorrent {
    typedef boost::system::error_code error_code;
}

namespace joystream {
namespace extension {

/**
 * @brief Base handler of result of request to a `PieceReaderInterface`.
 *
 * The handler is tied to a particular torrent, through its info_hash,
 * to prevent a single reader from reading pieces across torrents.
 */
class AbstractPieceReadHandler {

public:

    AbstractPieceReadHandler(const libtorrent::sha1_hash infoHash)
        : _infoHash(infoHash) {}

    /**
     * @brief Request to read piece with index `pieceIndex` was successfull, and it was of size `size` bytes, and is contained in `buffer`.
     * @param pieceIndex index of piece
     * @param size number of bytes in piece
     * @param buffer content of piece
     */
    virtual void pieceReadCompleted(int pieceIndex, int size, const boost::shared_array<char> & buffer) = 0;

    /**
     * @brief Request to read piece with index `pieceIndex` was successfull for reason described in `ec`.
     * @param pieceIndex index of piece
     * @param ec error code describing nature of failure
     */
    virtual void pieceReadFailed(int pieceIndex, const libtorrent::error_code & ec) = 0;

    /**
     * @brief Returns info_hash of torrent to which the handler reads pieces.
     * @return info_hash for torrent to which this handler reads pieces
     */
    libtorrent::sha1_hash infoHash() const noexcept {
        return _infoHash;
    }

private:

    // The info_hash of the torrent from which this handler
    // constricted to read pieces.
    const libtorrent::sha1_hash _infoHash;
};

namespace exception {

    /**
     * @brief Exception for when a handler is attempted registered multiple times with the same piece.
     */
    class HandlerAlreadyRegisteredWithPiece : public std::runtime_error {

    public:

        /**
         * @brief Constructor
         * @param pieceIndex
         * @param handler
         */
        HandlerAlreadyRegisteredWithPiece(int pieceIndex, const AbstractPieceReadHandler * handler)
            : std::runtime_error("")
            , _pieceIndex(pieceIndex)
            , _handler(handler) {}


        /**
         * @brief Returns piece index of registrartion attempt.
         * @return index of piece in registration attempt
         */
        int pieceIndex() const noexcept {
            return _pieceIndex;
        }

        /**
         * @brief Returns handler of registration attempt.
         * @return handler in registration attempt.
         */
        const AbstractPieceReadHandler * handler () const noexcept {
            return _handler;
        }

    private:

        int _pieceIndex;

        const AbstractPieceReadHandler * _handler;
    };
}

/**
 * @brief Interface for piece reading service.
 */
class PieceReaderInterface {

public:

    /**
     * @brief Register a request, on handler `requester`, to read piece with index `index`, from torrent specified by handler.
     * @param index index of piece
     * @param handler handler which handles result of request
     * @throws exception::HandlerAlreadyRegisteredWithPiece
     * @return number of pending registrations with given handler
     */
    virtual int readPiece(int pieceIndex, AbstractPieceReadHandler * handler) = 0;

    /**
     * @brief Cancel all registered requests with given handler.
     * @param handler handler for which to cancel all requests
     * @return number of cancelled registrations
     */
    virtual int cancelRequests(AbstractPieceReadHandler * handler) = 0;

};

}
}

#endif // JOYSTREAM_EXTENSION_PIECE_READER_PLUGIN_INTERFACE_HPP
