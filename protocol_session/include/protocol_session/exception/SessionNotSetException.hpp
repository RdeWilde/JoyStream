/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 10 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_EXCEPTION_SESSIONNOTSETEXCEPTION_HPP
#define JOYSTREAM_PROTOCOLSESSION_EXCEPTION_SESSIONNOTSETEXCEPTION_HPP

#include <stdexcept>

namespace joystream {
namespace protocol_session {
namespace exception {

    class SessionNotSetException : public std::runtime_error {

    public:

        SessionNotSetException()
            : std::runtime_error("Session was not set") {
        }

    };
}
}
}


#endif // JOYSTREAM_PROTOCOLSESSION_EXCEPTION_SESSIONNOTSETEXCEPTION_HPP
