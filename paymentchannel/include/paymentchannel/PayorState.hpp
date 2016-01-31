/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, January 29 2016
 */

#ifndef PAYMENT_CHANNEL_PAYOR_STATE_HPP
#define PAYMENT_CHANNEL_PAYOR_STATE_HPP

namespace joystream {
namespace paymentchannel {

    enum class PayorState {

        // Waiting to get PayeeConfiguration for all slots
        waiting_for_full_set_of_sellers,

        // Waiting to get refund signatures from all sellers
        waiting_for_full_set_of_refund_signatures,

        // Contract has been broadcasted, and at least one contract output
        // remains unspent and with sufficient balanc
        can_pay,

        // When
        all_contract_outputs_spent
    };

}
}

#endif // PAYMENT_CHANNEL_PAYOR_STATE_HPP

