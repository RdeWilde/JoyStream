/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 7 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_COMMON_HPP
#define JOYSTREAM_PROTOCOLSESSION_COMMON_HPP

#include <string>

namespace joystream {
namespace protocol_session {

template <class ConnectionIdType>
std::string IdToString(const ConnectionIdType &);

}
}
#endif // JOYSTREAM_PROTOCOLSESSION_COMMON_HPP
