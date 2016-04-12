/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 10 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_EXCEPTION_CONNECTIONALREADYADDEDEXCEPTION_HPP
#define JOYSTREAM_PROTOCOLSESSION_EXCEPTION_CONNECTIONALREADYADDEDEXCEPTION_HPP

#include <stdexcept>

namespace joystream {
namespace protocol_session {
namespace exception {

    template <class ConnectionIdType>
    class ConnectionAlreadyAddedException : public std::runtime_error {

    public:

        ConnectionAlreadyAddedException(const ConnectionIdType & id)
            : std::runtime_error(std::string("Connection with id ") + toString(id) + std::string(" already added."))
             , _id(id) {
        }

        ConnectionIdType id() const {
            return _id;
        }

    private:

        // Id of connection which has already been added
        ConnectionIdType _id;

    };

}
}
}

#endif // JOYSTREAM_PROTOCOLSESSION_EXCEPTION_CONNECTIONALREADYADDEDEXCEPTION_HPP

