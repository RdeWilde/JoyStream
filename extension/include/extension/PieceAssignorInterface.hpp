/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, November 27 2016
 */

#ifndef JOYSTREAM_EXTENSION_PIECE_ASSIGNOR_INTERFACE_HPP
#define JOYSTREAM_EXTENSION_PIECE_ASSIGNOR_INTERFACE_HPP

#include <libtorrent/socket.hpp>

namespace joystream {
namespace extension {

enum class PieceAssignmentStatus {

    // Cannot be assigned, e.g. if it has been downloaded
    Unassignable,

    // Piece is not currently assigned
    Unassigned,

    // Piece is currently assigned
    Assigned
};

class PieceAssignorInterface;

/**
 * @brief Assignment of a piece to a peer with a given end point.
 * It is mean to
 */
class PieceAssignment {

public:

    /**
     * @brief Returns index of assigned piece
     * @return index of piece
     */
    int pieceIndex() const noexcept {
        return _pieceIndex;
    }

    /**
     * @brief Returns endpoint of assignee
     * @return end point of assignee
     */
    libtorrent::tcp::endpoint assigneeEndPoint() const noexcept {
        return _assigneeEndPoint;
    }

private:

    friend class PieceAssignorInterface;

    PieceAssignment(int pieceIndex, libtorrent::tcp::endpoint _assigneeEndPoint)
        : _pieceIndex(pieceIndex){

    }

    // Index of assigned piece
    int _pieceIndex;

    // Endpoint of assignee
    libtorrent::tcp::endpoint _assigneeEndPoint;
};

namespace exception {

    class InvalidPieceIndex : public std::runtime_error {

    public:

        InvalidPieceIndex(int pieceIndex)
            : _pieceIndex(pieceIndex) {}

        int pieceIndex() const noexcept {
            return _pieceIndex;
        }

    private:

        int _pieceIndex;
    };

    class AllPiecesUnassignable : public std::runtime_error {

    public:

        AllPiecesUnassignable() : std::runtime_error("All pieces are unassignable.") {}
    };

    class NoUnassignedPieces : public std::runtime_error {

    public:

        NoUnassignedPieces() : std::runtime_error("No unassigned pieces.") {}
    };

    class PieceNotAssigned : public std::runtime_error {

    public:

        PieceNotAssigned(int pieceIndex)
            : std::runtime_error(message(pieceIndex))
            , _pieceIndex(pieceIndex) {}

    private:

        static std::string message(int pieceIndex) noexcept {
            return std::string("Piece ") + std::to_string(pieceIndex) + std::string(" not assigned.");
        }

        int _pieceIndex;
    };
}

/**
 * @brief Inteface for piece assignor.
 */
class PieceAssignorInterface {

public:

    /**
    * @brief Assignment status of piece with given index.
    * @param pieceIndex index of piece
    * @return status of piece
    * @throws exception::InvalidPieceIndex if index was not valid
    */
    virtual PieceAssignmentStatus status(unsigned int pieceIndex) const = 0;

    /**
    * @brief Assign an unassign piece to given assignee.
    * @param assignee to whom piece is to be assigned
    * @return assignment of a piece to the provided assignee
    * @throws exception::AllPiecesUnassignable if assignment status of all pieces are unassignable
    * @throws exception::NoUnassignedPieces all piece which are not unassignable, of which there is at least one, are currently assigned
    */
    virtual PieceAssignment assign(const libtorrent::tcp::endpoint & assignee) = 0;

    /**
    * @brief Makes an assigned piece unassigned
    * @param pieceAssignement piece assignment for piece to unassign
    * @throws exception::PieceNotAssigned if the piece is not currently assigned, means that the piece assignment has expired.
    */
    virtual void unassign(const PieceAssignment & pieceAssignement) = 0;

    /**
    * @brief Make assigned piece unassignable.
    * @param pieceAssignement piece assignment for piece to make unassignable
    * @throws exception::PieceNotAssigned if the piece is not currently assigned, means that the piece assignment has expired.
    */
    virtual void unassignable(const PieceAssignment & pieceAssignement) = 0;
};

}
}

#endif // JOYSTREAM_EXTENSION_PIECE_ASSIGNOR_INTERFACE_HPP
