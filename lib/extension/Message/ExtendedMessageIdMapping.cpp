#include "ExtendedMessageIdMapping.hpp"

#include <exception>

#define INVALID_EXTENDED_ID -1

ExtendedMessageIdMapping::ExtendedMessageIdMapping()
{
    for(int i = 0;i < Message::NUMBER_OF_MESSAGES;i++)
        ids[i] = INVALID_EXTENDED_ID; // just to have some non-random value
}

ExtendedMessageIdMapping::ExtendedMessageIdMapping(
                                                quint8 buyId,
                                                quint8 sellId,
                                                quint8 setupBeginId,
                                                quint8 setupBeginRejectId,
                                                quint8 setupContractId,
                                                quint8 setupContractSignedId,
                                                quint8 setupRefundId,
                                                quint8 setupRefundSignedId,
                                                quint8 setupContractPublishedId,
                                                quint8 setupCompletedId,
                                                quint8 pieceGetId,
                                                quint8 piecePutId,
                                                quint8 paymentId,
                                                quint8 endId) {

    ids[Message::buy] = buyId;
    ids[Message::sell] = sellId;
    ids[Message::setup_begin] = setupBeginId;
    ids[Message::setup_begin_reject] = setupBeginRejectId;
    ids[Message::setup_contract] = setupContractId;
    ids[Message::setup_contract_signed] = setupContractSignedId;
    ids[Message::setup_refund] = setupRefundId;
    ids[Message::setup_refund_signed] = setupRefundSignedId;
    ids[Message::setup_contract_published] = setupContractPublishedId;
    ids[Message::setup_completed] = setupCompletedId;
    ids[Message::piece_get] = pieceGetId;
    ids[Message::piece_put] = piecePutId;
    ids[Message::payment] = paymentId;
    ids[Message::end] = endId;
}

// Getters
void ExtendedMessageIdMapping::buy(quint8 id) { ids[Message::buy] = id;}
void ExtendedMessageIdMapping::sell(quint8 id) { ids[Message::sell] = id;}
void ExtendedMessageIdMapping::setup_begin(quint8 id) { ids[Message::setup_begin] = id;}
void ExtendedMessageIdMapping::setup_begin_reject(quint8 id) { ids[Message::setup_begin_reject] = id;}
void ExtendedMessageIdMapping::setup_contract(quint8 id) { ids[Message::setup_contract] = id;}
void ExtendedMessageIdMapping::setup_contract_signed(quint8 id) { ids[Message::setup_contract_signed] = id;}
void ExtendedMessageIdMapping::setup_refund(quint8 id) { ids[Message::setup_refund] = id;}
void ExtendedMessageIdMapping::setup_refund_signed(quint8 id) { ids[Message::setup_refund_signed] = id;}
void ExtendedMessageIdMapping::setup_contract_published(quint8 id) { ids[Message::setup_contract_published] = id;}
void ExtendedMessageIdMapping::setup_completed(quint8 id) { ids[Message::setup_completed] = id;}
void ExtendedMessageIdMapping::piece_get(quint8 id) { ids[Message::piece_get] = id;}
void ExtendedMessageIdMapping::piece_put(quint8 id) { ids[Message::piece_put] = id;}
void ExtendedMessageIdMapping::payment(quint8 id) { ids[Message::payment] = id;}
void ExtendedMessageIdMapping::end(quint8 id) { ids[Message::end] = id;}

// Setters
quint8 ExtendedMessageIdMapping::buy() const { return ids[Message::buy];}
quint8 ExtendedMessageIdMapping::sell() const { return ids[Message::sell];}
quint8 ExtendedMessageIdMapping::setup_begin() const { return ids[Message::setup_begin];}
quint8 ExtendedMessageIdMapping::setup_begin_reject() const { return ids[Message::setup_begin_reject];}
quint8 ExtendedMessageIdMapping::setup_contract() const { return ids[Message::setup_contract];}
quint8 ExtendedMessageIdMapping::setup_contract_signed() const { return ids[Message::setup_contract_signed];}
quint8 ExtendedMessageIdMapping::setup_refund() const { return ids[Message::setup_refund];}
quint8 ExtendedMessageIdMapping::setup_refund_signed() const { return ids[Message::setup_refund_signed];}
quint8 ExtendedMessageIdMapping::setup_contract_published() const { return ids[Message::setup_contract_published];}
quint8 ExtendedMessageIdMapping::setup_completed() const { return ids[Message::setup_completed];}
quint8 ExtendedMessageIdMapping::piece_get() const { return ids[Message::piece_get];}
quint8 ExtendedMessageIdMapping::piece_put() const { return ids[Message::piece_put];}
quint8 ExtendedMessageIdMapping::payment() const { return ids[Message::payment];}
quint8 ExtendedMessageIdMapping::end() const { return ids[Message::end];}

// Get message corresponding to id
Message::TYPE ExtendedMessageIdMapping::getMessageType(quint8 id) const {

    // Find match: NB!! Assumes enum values are in order.
    for(quint8 i = Message::buy; i <= Message::end;i++) {

        if(id == ids[i])
            return static_cast<Message::TYPE>(i);
    }

    // Throw exception
    throw std::exception("Invalid id passed");
}

// Get message corresponding to message name
Message::TYPE ExtendedMessageIdMapping::getMessageType(std::string & messageName) {

    if(messageName == "buy")
        return Message::buy;
    else if(messageName == "sell")
        return Message::sell;
    else if(messageName == "setup_begin")
        return Message::setup_begin;
    else if(messageName == "setup_begin_reject")
        return Message::setup_begin_reject;
    else if(messageName == "setup_contract")
        return Message::setup_contract;
    else if(messageName == "setup_contract_signed")
        return Message::setup_contract_signed;
    else if(messageName == "setup_refund")
        return Message::setup_refund;
    else if(messageName == "setup_refund_signed")
        return Message::setup_refund_signed;
    else if(messageName == "setup_contract_published")
        return Message::setup_contract_published;
    else if(messageName == "setup_completed")
        return Message::setup_completed;
    else if(messageName == "piece_get")
        return Message::piece_get;
    else if(messageName == "piece_put")
        return Message::piece_put;
    else if(messageName == "payment")
        return Message::payment;
    else if(messageName == "end")
        return Message::end;
    else
        throw std::exception("Invalid message passed");
}

// Get name of message
std::string ExtendedMessageIdMapping::getMessageName(Message::TYPE type) {

    switch(type) {
        case Message::buy: return "buy";
        case Message::sell: return "sell";
        case Message::setup_begin: return "setup_begin";
        case Message::setup_begin_reject: return "setup_begin_reject";
        case Message::setup_contract: return "setup_contract";
        case Message::setup_contract_signed: return "setup_contract_signed";
        case Message::setup_refund: return "setup_refund";
        case Message::setup_refund_signed: return "setup_refund_signed";
        case Message::setup_contract_published: return "setup_contract_published";
        case Message::setup_completed: return "setup_completed";
        case Message::piece_get: return "piece_get";
        case Message::piece_put: return "piece_put";
        case Message::payment: return "payment";
        case Message::end: return "end";
    }
}

// Check validity of mapping
bool ExtendedMessageIdMapping::isValid() const {

    // Check that
    for(int i = 0;i < Message::NUMBER_OF_MESSAGES;i++) {

        // Check if this id is valid
        if(ids[i] == INVALID_EXTENDED_ID)
            return false;

        // Check if there is other matching ids
        for(int j = i + 1;j < Message::NUMBER_OF_MESSAGES;j++) {

            // Two messages have same id
            if(ids[i] == ids[j])
                return false;
        }
    }

    // No two message have same id
    return true;
}
