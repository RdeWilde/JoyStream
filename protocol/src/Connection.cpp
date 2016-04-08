/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, February 9 2016
 */

#include <protocol/Connection.hpp>

namespace joystream {
namespace protocol {

    template <class ConnectionIdType>
    Connection<ConnectionIdType>::Connection() {
    }

    template <class ConnectionIdType>
    Connection<ConnectionIdType>::Connection(const ConnectionIdType & connectionId)
        : _connectionId(connectionId){
    }
}
}
