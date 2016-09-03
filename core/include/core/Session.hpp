/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_SESSION_HPP
#define JOYSTREAM_CORE_SESSION_HPP

#include <protocol_session/protocol_session.hpp>
#include <core/Connection.hpp>
#include <core/Selling.hpp>
#include <core/Buying.hpp>

#include <libtorrent/socket.hpp>

#include <QObject>

namespace joystream {
namespace core {

class Session : public QObject {

    Q_OBJECT

private:

    Session(const protocol_session::status::Session<libtorrent::tcp::endpoint> &);

public:

    protocol_session::SessionMode mode() const noexcept;

    protocol_session::SessionState state() const noexcept;

    std::map<libtorrent::tcp::endpoint, Connection *> connections() const noexcept;

    Selling * selling() const noexcept;

    Buying * buying() const noexcept;

signals:

    void modeChanged(protocol_session::SessionMode);

    void stateChanged(protocol_session::SessionState);

    void connectionAdded(const Connection *);

    void connectionRemoved(const libtorrent::tcp::endpoint &);

private:

    friend class TorrentPlugin;

    void addConnection(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status);

    void removeConnection(const libtorrent::tcp::endpoint &);

    void update(const protocol_session::status::Session<libtorrent::tcp::endpoint> &);

    // Session mode
    protocol_session::SessionMode _mode;

    // Current state of session
    protocol_session::SessionState _state;

    // Connections
    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Connection> > _connections;

    /// Substates

    // Selling mode
    std::unique_ptr<Selling> _selling;

    // Buying mode
    std::unique_ptr<Buying> _buying;

    // If mode has not changed, then status is updated, if it has, then old substate is discarded
    // (if it was buying or selling) and mode change signal is emitted
    void updateSubstate(const protocol_session::status::Session<libtorrent::tcp::endpoint> & status);
};

}
}

#endif // JOYSTREAM_CORE_SESSION_HPP
