#include "ExtendedMessageIdMapping.hpp"

#include <set>
#include <exception>

#define INVALID_EXTENDED_ID -1

ExtendedMessageIdMapping::ExtendedMessageIdMapping()
    : buyId_(INVALID_EXTENDED_ID)
    , sellId_(INVALID_EXTENDED_ID)
    , setupBeginId_(INVALID_EXTENDED_ID)
    , setupBeginRejectId_(INVALID_EXTENDED_ID)
    , setupContractId_(INVALID_EXTENDED_ID)
    , setupContractSignedId_(INVALID_EXTENDED_ID)
    , setupRefundId_(INVALID_EXTENDED_ID)
    , setupRefundSignedId_(INVALID_EXTENDED_ID)
    , setupContractPublishedId_(INVALID_EXTENDED_ID)
    , setupCompletedId_(INVALID_EXTENDED_ID)
    , pieceGetId_(INVALID_EXTENDED_ID)
    , piecePutId_(INVALID_EXTENDED_ID)
    , paymentId_(INVALID_EXTENDED_ID)
    , endId_(INVALID_EXTENDED_ID)
{ }

ExtendedMessageIdMapping::ExtendedMessageIdMapping(const libtorrent::lazy_entry * m) {

    buy(m->dict_find_int_value(getMessageName(MessageType::buy).c_str(), INVALID_EXTENDED_ID));
    sell(m->dict_find_int_value(getMessageName(MessageType::sell).c_str(), INVALID_EXTENDED_ID));
    setup_begin(m->dict_find_int_value(getMessageName(MessageType::setup_begin).c_str(), INVALID_EXTENDED_ID));
    setup_begin_reject(m->dict_find_int_value(getMessageName(MessageType::setup_begin_reject).c_str(), INVALID_EXTENDED_ID));
    setup_contract(m->dict_find_int_value(getMessageName(MessageType::setup_contract).c_str(), INVALID_EXTENDED_ID));
    setup_contract_signed(m->dict_find_int_value(getMessageName(MessageType::setup_contract_signed).c_str(), INVALID_EXTENDED_ID));
    setup_refund(m->dict_find_int_value(getMessageName(MessageType::setup_refund).c_str(), INVALID_EXTENDED_ID));
    setup_refund_signed(m->dict_find_int_value(getMessageName(MessageType::setup_refund_signed).c_str(), INVALID_EXTENDED_ID));
    setup_contract_published(m->dict_find_int_value(getMessageName(MessageType::setup_contract_published).c_str(), INVALID_EXTENDED_ID));
    setup_completed(m->dict_find_int_value(getMessageName(MessageType::setup_completed).c_str(), INVALID_EXTENDED_ID));
    piece_get(m->dict_find_int_value(getMessageName(MessageType::piece_get).c_str(), INVALID_EXTENDED_ID));
    piece_put(m->dict_find_int_value(getMessageName(MessageType::piece_put).c_str(), INVALID_EXTENDED_ID));
    payment(m->dict_find_int_value(getMessageName(MessageType::payment).c_str(), INVALID_EXTENDED_ID));
    end(m->dict_find_int_value(getMessageName(MessageType::end).c_str(), INVALID_EXTENDED_ID));
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
                                                quint8 endId)
    : buyId_(buyId)
    , sellId_(sellId)
    , setupBeginId_(setupBeginId)
    , setupBeginRejectId_(setupBeginRejectId)
    , setupContractId_(setupContractId)
    , setupContractSignedId_(setupContractSignedId)
    , setupRefundId_(setupRefundId)
    , setupRefundSignedId_(setupRefundSignedId)
    , setupContractPublishedId_(setupContractPublishedId)
    , setupCompletedId_(setupCompletedId)
    , pieceGetId_(pieceGetId)
    , piecePutId_(piecePutId)
    , paymentId_(paymentId)
    , endId_(endId)
{ }

void ExtendedMessageIdMapping::writeToDictionary(libtorrent::entry::dictionary_type & m) {

    m[getMessageName(MessageType::buy)] = buy();
    m[getMessageName(MessageType::sell)] = sell();
    m[getMessageName(MessageType::setup_begin)] = setup_begin();
    m[getMessageName(MessageType::setup_begin_reject)] = setup_begin_reject();
    m[getMessageName(MessageType::setup_contract)] = setup_contract();
    m[getMessageName(MessageType::setup_contract_signed)] = setup_contract_signed();
    m[getMessageName(MessageType::setup_refund)] = setup_refund();
    m[getMessageName(MessageType::setup_refund_signed)] = setup_refund_signed();
    m[getMessageName(MessageType::setup_contract_published)] = setup_contract_published();
    m[getMessageName(MessageType::setup_completed)] = setup_completed();
    m[getMessageName(MessageType::piece_get)] = piece_get();
    m[getMessageName(MessageType::piece_put)] = piece_put();
    m[getMessageName(MessageType::payment)] = payment();
    m[getMessageName(MessageType::end)] = end();
}

// Getters
void ExtendedMessageIdMapping::buy(quint8 id) { buyId_ = id;}
void ExtendedMessageIdMapping::sell(quint8 id) { sellId_ = id;}
void ExtendedMessageIdMapping::setup_begin(quint8 id) { setupBeginId_ = id;}
void ExtendedMessageIdMapping::setup_begin_reject(quint8 id) { setupBeginRejectId_ = id;}
void ExtendedMessageIdMapping::setup_contract(quint8 id) { setupContractId_ = id;}
void ExtendedMessageIdMapping::setup_contract_signed(quint8 id) { setupContractSignedId_ = id;}
void ExtendedMessageIdMapping::setup_refund(quint8 id) { setupRefundId_ = id;}
void ExtendedMessageIdMapping::setup_refund_signed(quint8 id) { setupRefundSignedId_ = id;}
void ExtendedMessageIdMapping::setup_contract_published(quint8 id) { setupContractPublishedId_ = id;}
void ExtendedMessageIdMapping::setup_completed(quint8 id) { setupCompletedId_ = id;}
void ExtendedMessageIdMapping::piece_get(quint8 id) { pieceGetId_ = id;}
void ExtendedMessageIdMapping::piece_put(quint8 id) { piecePutId_ = id;}
void ExtendedMessageIdMapping::payment(quint8 id) { paymentId_ = id;}
void ExtendedMessageIdMapping::end(quint8 id) { endId_ = id;}

// Setters
quint8 ExtendedMessageIdMapping::buy() const { return buyId_;}
quint8 ExtendedMessageIdMapping::sell() const { return sellId_;}
quint8 ExtendedMessageIdMapping::setup_begin() const { return setupBeginId_;}
quint8 ExtendedMessageIdMapping::setup_begin_reject() const { return setupBeginRejectId_;}
quint8 ExtendedMessageIdMapping::setup_contract() const { return setupContractId_;}
quint8 ExtendedMessageIdMapping::setup_contract_signed() const { return setupContractSignedId_;}
quint8 ExtendedMessageIdMapping::setup_refund() const { return setupRefundId_;}
quint8 ExtendedMessageIdMapping::setup_refund_signed() const { return setupRefundSignedId_;}
quint8 ExtendedMessageIdMapping::setup_contract_published() const { return setupContractPublishedId_;}
quint8 ExtendedMessageIdMapping::setup_completed() const { return setupCompletedId_;}
quint8 ExtendedMessageIdMapping::piece_get() const { return pieceGetId_;}
quint8 ExtendedMessageIdMapping::piece_put() const { return piecePutId_;}
quint8 ExtendedMessageIdMapping::payment() const { return paymentId_;}
quint8 ExtendedMessageIdMapping::end() const { return endId_;}

// Get message corresponding to id
MessageType ExtendedMessageIdMapping::getMessageType(quint8 id) const {

    if(id == buyId_)
        return MessageType::buy;
    else if(id == sellId_)
        return MessageType::sell;
    else if(id == setupBeginId_)
        return MessageType::setup_begin;
    else if(id == setupBeginRejectId_)
        return MessageType::setup_begin_reject;
    else if(id == setupContractId_)
        return MessageType::setup_contract;
    else if(id == setupContractSignedId_)
        return MessageType::setup_contract_signed;
    else if(id == setupRefundId_)
        return MessageType::setup_refund;
    else if(id == setupRefundSignedId_)
        return MessageType::setup_refund_signed;
    else if(id == setupContractPublishedId_)
        return MessageType::setup_contract_published;
    else if(id == setupCompletedId_)
        return MessageType::setup_completed;
    else if(id == pieceGetId_)
        return MessageType::piece_get;
    else if(id == piecePutId_)
        return MessageType::piece_put;
    else if(id == paymentId_)
        return MessageType::payment;
    else if(id == endId_)
        return MessageType::end;
    else // Throw exception
        throw std::exception("Invalid id passed");
}

// Get message corresponding to message name
MessageType ExtendedMessageIdMapping::getMessageType(std::string & messageName) {

    if(messageName == "buy")
        return MessageType::buy;
    else if(messageName == "sell")
        return MessageType::sell;
    else if(messageName == "setup_begin")
        return MessageType::setup_begin;
    else if(messageName == "setup_begin_reject")
        return MessageType::setup_begin_reject;
    else if(messageName == "setup_contract")
        return MessageType::setup_contract;
    else if(messageName == "setup_contract_signed")
        return MessageType::setup_contract_signed;
    else if(messageName == "setup_refund")
        return MessageType::setup_refund;
    else if(messageName == "setup_refund_signed")
        return MessageType::setup_refund_signed;
    else if(messageName == "setup_contract_published")
        return MessageType::setup_contract_published;
    else if(messageName == "setup_completed")
        return MessageType::setup_completed;
    else if(messageName == "piece_get")
        return MessageType::piece_get;
    else if(messageName == "piece_put")
        return MessageType::piece_put;
    else if(messageName == "payment")
        return MessageType::payment;
    else if(messageName == "end")
        return MessageType::end;
    else
        throw std::exception("Invalid message passed");
}

// Get name of message
std::string ExtendedMessageIdMapping::getMessageName(MessageType type) {

    switch(type) {
        case MessageType::buy: return "buy";
        case MessageType::sell: return "sell";
        case MessageType::setup_begin: return "setup_begin";
        case MessageType::setup_begin_reject: return "setup_begin_reject";
        case MessageType::setup_contract: return "setup_contract";
        case MessageType::setup_contract_signed: return "setup_contract_signed";
        case MessageType::setup_refund: return "setup_refund";
        case MessageType::setup_refund_signed: return "setup_refund_signed";
        case MessageType::setup_contract_published: return "setup_contract_published";
        case MessageType::setup_completed: return "setup_completed";
        case MessageType::piece_get: return "piece_get";
        case MessageType::piece_put: return "piece_put";
        case MessageType::payment: return "payment";
        case MessageType::end: return "end";
    }
}

// Check validity of mapping
bool ExtendedMessageIdMapping::isValid() const {

    // Check
    if(INVALID_EXTENDED_ID == buyId_)
        return false;
    else if(INVALID_EXTENDED_ID == sellId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupBeginId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupBeginRejectId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupContractId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupContractSignedId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupRefundId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupRefundSignedId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupContractPublishedId_)
        return false;
    else if(INVALID_EXTENDED_ID == setupCompletedId_)
        return false;
    else if(INVALID_EXTENDED_ID == pieceGetId_)
        return false;
    else if(INVALID_EXTENDED_ID == piecePutId_)
        return false;
    else if(INVALID_EXTENDED_ID == paymentId_)
        return false;
    else if(INVALID_EXTENDED_ID == endId_)
        return false;

    quint8 ids [] = { buyId_,
                        sellId_,
                        setupBeginId_,
                        setupBeginRejectId_,
                        setupContractId_,
                        setupContractSignedId_,
                        setupRefundId_,
                        setupRefundSignedId_,
                        setupContractPublishedId_,
                        setupCompletedId_,
                        pieceGetId_,
                        piecePutId_,
                        paymentId_,
                        endId_};

    std::set<quint8> uniqueness(ids, ids + 14);

    // Do we have the correct number of unique ids?
    if(uniqueness.size() != NUMBER_OF_MESSAGES)
        return false;

    // No two message have same id, and no one has id INVALID_EXTENDED_ID
    return true;
}

void ExtendedMessageIdMapping::setAllStartingAt(quint8 id) {

    buy(id++);
    sell(id++);
    setup_begin(id++);
    setup_begin_reject(id++);
    setup_contract(id++);
    setup_contract_signed(id++);
    setup_refund(id++);
    setup_refund_signed(id++);
    setup_contract_published(id++);
    setup_completed(id++);
    piece_get(id++);
    piece_put(id++);
    payment(id++);
    end(id++);
}
