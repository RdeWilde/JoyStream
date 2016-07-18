/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 28 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_SELLINGPOLICY_HPP
#define JOYSTREAM_PROTOCOLSESSION_SELLINGPOLICY_HPP

namespace joystream {
namespace protocol_session {

class SellingPolicy {

public:

    bool operator==(const SellingPolicy &) const {
        return true;
    }

private:

};

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_SELLINGPOLICY_HPP
