/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <protocol_wire/MessageType.hpp>
#include <protocol_wire/Observe.hpp>
#include <protocol_wire/Buy.hpp>
#include <protocol_wire/Sell.hpp>
#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/JoiningContract.hpp>
#include <protocol_wire/SignRefund.hpp>
#include <protocol_wire/RefundSigned.hpp>
#include <protocol_wire/Ready.hpp>
#include <protocol_wire/RequestFullPiece.hpp>
#include <protocol_wire/FullPiece.hpp>
#include <protocol_wire/Payment.hpp>

#include <QDataStream>

namespace joystream {
namespace protocol_wire {

    ExtendedMessagePayload * ExtendedMessagePayload::fromRaw(MessageType type, QDataStream & stream, int lengthOfExtendedMessagePayload) {

        try {

            switch(type) {

                case MessageType::observe: return new Observe();
                case MessageType::buy: return new Buy(stream);
                case MessageType::sell: return new Sell(stream);
                case MessageType::join_contract: return new JoinContract();
                case MessageType::joining_contract: return new JoiningContract(stream);
                case MessageType::sign_refund: return new SignRefund(stream);
                case MessageType::refund_signed: return new RefundSigned(stream, lengthOfExtendedMessagePayload);
                case MessageType::ready: return new Ready();
                case MessageType::request_full_piece: return new RequestFullPiece(stream);
                case MessageType::full_piece: return new FullPiece(stream, lengthOfExtendedMessagePayload);
                case MessageType::payment: return new Payment(stream, lengthOfExtendedMessagePayload);

                default:
                    Q_ASSERT(false); // We are not covering full value range of enum
                    return NULL;
            }

        } catch (std::exception & e) {
            return NULL;
        }
    }
}
}
