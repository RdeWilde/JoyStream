/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 24 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_EXCEPTIONS_HPP
#define JOYSTREAM_PROTOCOLSESSION_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

template <class ConnectionIdType>
std::string IdToString(const ConnectionIdType &);

namespace joystream {
namespace protocol_session {
namespace exception {

template <class ConnectionIdType>
class ConnectionAlreadyAddedException : public std::runtime_error {

public:

    ConnectionAlreadyAddedException(const ConnectionIdType & id)
        : std::runtime_error(std::string("Connection with id ") + IdToString(id) + std::string(" already added."))
        , _id(id) {
    }

    ConnectionIdType id() const {
        return _id;
    }

private:

    // Id of connection which has already been added
    ConnectionIdType _id;

};

class InvalidPieceIndexException : public std::runtime_error {

public:

    InvalidPieceIndexException(int expectedPieceIndex, int actualPieceIndex)
        : std::runtime_error("Expected piece index ") // + std::string(expectedPieceIndex))
        , _expectedPieceIndex(expectedPieceIndex)
        , _actualPieceIndex(actualPieceIndex) {
    }

    int expectedPieceIndex() const {
        return _expectedPieceIndex;
    }

    int actualPieceIndex() const {
        return _actualPieceIndex;
    }

private:

    int _expectedPieceIndex, _actualPieceIndex;

};

template <class ConnectionIdType>
class ConnectionDoesNotExist : public std::runtime_error {

public:

    ConnectionDoesNotExist(const ConnectionIdType & id)
        : std::runtime_error(std::string("Connection with id ") + IdToString(id) + std::string(" does not exist."))
         , _id(id) {
    }

    ConnectionIdType id() const {
        return _id;
    }

private:

    ConnectionIdType _id;

};

class StateIncompatibleOperation : public std::runtime_error {

public:

    StateIncompatibleOperation()
        : std::runtime_error("Operation not compatible with current state.") {
    }
};

class SessionModeNotSetException : public std::runtime_error {

public:

    SessionModeNotSetException()
        : std::runtime_error("Mode of session not yet set.") {
    }

};

class SessionAlreadyInThisMode : public std::runtime_error {

public:

    SessionAlreadyInThisMode()
        : std::runtime_error("Session is already in this mode.") {
    }
};

class ModeIncompatibleOperation : public std::runtime_error {

public:

    ModeIncompatibleOperation()
        : std::runtime_error("Operation is incompatible with current session mode.") {
    }
};

}
}
}

#endif // JOYSTREAM_PROTOCOLSESSION_EXCEPTIONS_HPP
