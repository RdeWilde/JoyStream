#include "ExtendedMessagePayload.hpp"
#include "MessageType.hpp"
#include "Observe.hpp"
#include "Buy.hpp"
#include "Sell.hpp"
#include "JoinContract.hpp"
#include "JoiningContract.hpp"
#include "SignRefund.hpp"
#include "RefundSigned.hpp"
#include "Ready.hpp"
#include "RequestFullPiece.hpp"
#include "FullPiece.hpp"
#include "Payment.hpp"

#include <QDataStream>

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
