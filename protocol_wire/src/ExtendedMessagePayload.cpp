/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <cassert>

#include <protocol_wire/ExtendedMessagePayload.hpp>
#include <protocol_wire/MessageType.hpp>
#include <protocol_wire/Observe.hpp>
#include <protocol_wire/Buy.hpp>
#include <protocol_wire/Sell.hpp>
#include <protocol_wire/JoinContract.hpp>
#include <protocol_wire/JoiningContract.hpp>
#include <protocol_wire/Ready.hpp>
#include <protocol_wire/RequestFullPiece.hpp>
#include <protocol_wire/FullPiece.hpp>
#include <protocol_wire/Payment.hpp>

namespace joystream {
namespace protocol_wire {

    ExtendedMessagePayload * ExtendedMessagePayload::fromRaw(MessageType type, std::istream & stream, int lengthOfExtendedMessagePayload) {

        try {

            switch(type) {

                case MessageType::observe: return new Observe();
                case MessageType::buy: return new Buy(stream);
                case MessageType::sell: return new Sell(stream);
                case MessageType::join_contract: return new JoinContract(stream);
                case MessageType::joining_contract: return new JoiningContract(stream);
                case MessageType::ready: return new Ready(stream);
                case MessageType::request_full_piece: return new RequestFullPiece(stream);
                case MessageType::full_piece: return new FullPiece(stream, lengthOfExtendedMessagePayload);
                case MessageType::payment: return new Payment(stream, lengthOfExtendedMessagePayload);

                default:
                    assert(false); // We are not covering full value range of enum
                    return NULL;
            }

        } catch (std::exception & e) {
            return NULL;
        }
    }

    std::shared_ptr<ExtendedMessagePayload> ExtendedMessagePayload::fromWireStream(MessageType type, InputWireStream & stream) {
        std::shared_ptr<ExtendedMessagePayload> m;

        switch(type) {

        case MessageType::observe: {
                m.reset(new protocol_wire::Observe());
                stream >> *dynamic_cast<protocol_wire::Observe*>(m.get());
                break;
            }
        case MessageType::buy: {
                m.reset(new protocol_wire::Buy());
                stream >> *dynamic_cast<protocol_wire::Buy*>(m.get());
                break;
            }
        case MessageType::sell: {
                m.reset(new protocol_wire::Sell());
                stream >> *dynamic_cast<protocol_wire::Sell*>(m.get());
                break;
            }
        case MessageType::join_contract: {
                m.reset(new protocol_wire::JoinContract());
                stream >> *dynamic_cast<protocol_wire::JoinContract*>(m.get());
                break;
            }
        case MessageType::joining_contract: {
                m.reset(new protocol_wire::JoiningContract());
                stream >> *dynamic_cast<protocol_wire::JoiningContract*>(m.get());
                break;
            }
        case MessageType::ready: {
                m.reset(new protocol_wire::Ready());
                stream >> *dynamic_cast<protocol_wire::Ready*>(m.get());
                break;
            }
        case MessageType::request_full_piece: {
                m.reset(new protocol_wire::RequestFullPiece());
                stream >> *dynamic_cast<protocol_wire::RequestFullPiece*>(m.get());
                break;
            }
        case MessageType::full_piece: {
                m.reset(new protocol_wire::FullPiece());
                stream >> *dynamic_cast<protocol_wire::FullPiece*>(m.get());
                break;
            }
        case MessageType::payment: {
                m.reset(new protocol_wire::Payment());
                stream >> *dynamic_cast<protocol_wire::Payment*>(m.get());
                break;
            }

        default:
            assert(false); // We are not covering full value range of enum
       }

        return m;
    }
}
}
