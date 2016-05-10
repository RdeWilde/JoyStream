/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, March 17 2016
 */

#ifndef JOYSTREAM_PROTOCOLSTATEMACHINE_INVITEDTOJOINCONTRACTBYNONBUYER_HPP
#define JOYSTREAM_PROTOCOLSTATEMACHINE_INVITEDTOJOINCONTRACTBYNONBUYER_HPP

#include <protocol_statemachine/AnnouncedModeAndTerms.hpp>
#include <stdexcept>

namespace joystream {
namespace protocol_statemachine {

    enum class ModeAnnounced;

    namespace exception {

        class InvitedToJoinContractByNonBuyer : public std::runtime_error {

        public:

            enum class NonBuyModeAnnounced {
                None,
                Sell,
                Observe
            };

            static NonBuyModeAnnounced toNonBuyMode(ModeAnnounced mode);

            InvitedToJoinContractByNonBuyer(NonBuyModeAnnounced mode);
            InvitedToJoinContractByNonBuyer(ModeAnnounced mode);

            // Getters and setters
            NonBuyModeAnnounced peerMode() const;
            void setPeerMode(NonBuyModeAnnounced peerMode);

        private:

            // The actual mode the peer was in
            NonBuyModeAnnounced _peerMode;
        };
    }
}
}

#endif // JOYSTREAM_PROTOCOLSTATEMACHINE_INVITEDTOJOINCONTRACTBYNONBUYER_HPP

