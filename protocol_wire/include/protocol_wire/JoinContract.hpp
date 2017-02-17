/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef JOYSTREAM_PROTOCOL_WIRE_JOINCONTRACT_HPP
#define JOYSTREAM_PROTOCOL_WIRE_JOINCONTRACT_HPP

#include <stdint.h>

namespace joystream {
namespace protocol_wire {

    class JoinContract {

    public:

        JoinContract();
        JoinContract(uint32_t index);

        bool operator==(const JoinContract &) const;

        // Getters
        uint32_t index() const;

        void setIndex(uint32_t);
    private:

        // Seller terms index
        uint32_t _index;
    };

}
}

#endif // JOYSTREAM_PROTOCOL_WIRE_JOINCONTRACT_HPP
