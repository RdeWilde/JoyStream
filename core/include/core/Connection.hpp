/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_CONNECTION_HPP
#define JOYSTREAM_CORE_CONNECTION_HPP

#include <libtorrent/socket_io.hpp>

#include <QObject>

#include <memory>

namespace joystream {
namespace protocol_session {
namespace status {
    template <class ConnectionIdType>
    struct Connection;
}
}

namespace core {

class CBStateMachine;

class Connection : public QObject {

    Q_OBJECT

public:

    Connection(const libtorrent::tcp::endpoint & connectionId,
               CBStateMachine * machine);

    static Connection * create(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status);

    ~Connection();

    libtorrent::tcp::endpoint connectionId() const noexcept;

    CBStateMachine * machine() const noexcept;

signals:

private:

    friend class Session;

    void update(const protocol_session::status::Connection<libtorrent::tcp::endpoint> &);

    // Connection id
    libtorrent::tcp::endpoint _connectionId;

    // State machine for this connection
    std::unique_ptr<CBStateMachine> _machine;
};

}
}

#endif // JOYSTREAM_CORE_CONNECTION_HPP
