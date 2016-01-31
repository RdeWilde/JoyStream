/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef PAYMENT_CHANNEL_CHANNEL_STATE_HPP
#define PAYMENT_CHANNEL_CHANNEL_STATE_HPP

namespace joystream {
namespace paymentchannel {

    enum class ChannelState {
        unassigned,
        assigned,
        refund_signed
    };

}
}

#endif // PAYMENT_CHANNEL_CHANNEL_STATE_HPP

