/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_CONNECTION_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_CONNECTION_HPP

#include <controller/CBStateMachine.hpp>

#include <QObject>

namespace joystream {
namespace core {
    class Connection;
}
namespace classic {
namespace gui {
    class PeersDialog;
    class TorrentTableRowModel;
}
namespace controller {

class Connection : public QObject {

    Q_OBJECT

public:

    Connection(core::Connection * connection);

    CBStateMachine & stateMachine() noexcept;

public slots:

private:

    core::Connection * _connection;

    CBStateMachine _stateMachine;

};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_CONNECTION_HPP
