/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 29 2016
 */

#ifndef PAYMENT_CHANNEL_PAYEE_STATE_HPP
#define PAYMENT_CHANNEL_PAYEE_STATE_HPP

namespace joystream {
namespace paymentchannel {

    /**
     * @brief The payee state
     */
    enum class PayeeState {

        // Information, provided by payee, required for receiving payments has not been set (using registerPayeeInformation)
        // WHY DO WE EVEN NEED THIS, WE CONTROL THIS INFORMATION ANYWAY
        // waiting_for_payee_information,

        // Information, provided by payor, required for receiving payments has not been set (using registerPayorInformation)
        waiting_for_payor_information,

        // All information required to sign refunds and validate payments has been provided
        has_all_information_required,

        /**
        //
        final_payment_spent,

        //
        refund_spent,
        */
    };

}
}

#endif // PAYMENT_CHANNEL_PAYEE_STATE_HPP

