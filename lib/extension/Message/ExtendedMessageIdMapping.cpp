#include "ExtendedMessageIdMapping.hpp"

ExtendedMessageIdMapping::ExtendedMessageIdMapping()
{
    for(int i = 0;i < NUMBER_OF_MESSAGES;i++)
        ids[i] = -1; // just to have some non-random value
}

ExtendedMessageIdMapping::ExtendedMessageIdMapping(
                                                uint8_t buyId,
                                                uint8_t sellId,
                                                uint8_t setupBeginId,
                                                uint8_t setupBeginRejectId,
                                                uint8_t setupContractId,
                                                uint8_t setupContractSignedId,
                                                uint8_t setupRefundId,
                                                uint8_t setupRefundSignedId,
                                                uint8_t setupContractPublishedId,
                                                uint8_t setupCompletedId,
                                                uint8_t pieceGetId,
                                                uint8_t piecePutId,
                                                uint8_t paymentId,
                                                uint8_t endId) {

    ids[0] = buyId;
    ids[1] = sellId;
    ids[2] = setupBeginId;
    ids[3] = setupBeginRejectId;
    ids[4] = setupContractId;
    ids[5] = setupContractSignedId;
    ids[6] = setupRefundId;
    ids[7] = setupRefundSignedId;
    ids[8] = setupContractPublishedId;
    ids[9] = setupCompletedId;
    ids[10] = pieceGetId;
    ids[11] = piecePutId;
    ids[12] = paymentId;
    ids[13] = endId;
}

// Getters
void ExtendedMessageIdMapping::buy(uint8_t id) { ids[0] = id;}
void ExtendedMessageIdMapping::sell(uint8_t id) { ids[1] = id;}
void ExtendedMessageIdMapping::setup_begin(uint8_t id) { ids[2] = id;}
void ExtendedMessageIdMapping::setup_begin_reject(uint8_t id) { ids[3] = id;}
void ExtendedMessageIdMapping::setup_contract(uint8_t id) { ids[4] = id;}
void ExtendedMessageIdMapping::setup_contract_signed(uint8_t id) { ids[5] = id;}
void ExtendedMessageIdMapping::setup_refund(uint8_t id) { ids[6] = id;}
void ExtendedMessageIdMapping::setup_refund_signed(uint8_t id) { ids[7] = id;}
void ExtendedMessageIdMapping::setup_contract_published(uint8_t id) { ids[8] = id;}
void ExtendedMessageIdMapping::setup_completed(uint8_t id) { ids[9] = id;}
void ExtendedMessageIdMapping::piece_get(uint8_t id) { ids[10] = id;}
void ExtendedMessageIdMapping::piece_put(uint8_t id) { ids[11] = id;}
void ExtendedMessageIdMapping::payment(uint8_t id) { ids[12] = id;}
void ExtendedMessageIdMapping::end(uint8_t id) { ids[13] = id;}

// Setters
uint8_t ExtendedMessageIdMapping::buy() const { return ids[0];}
uint8_t ExtendedMessageIdMapping::sell() const { return ids[1];}
uint8_t ExtendedMessageIdMapping::setup_begin() const { return ids[2];}
uint8_t ExtendedMessageIdMapping::setup_begin_reject() const { return ids[3];}
uint8_t ExtendedMessageIdMapping::setup_contract() const { return ids[4];}
uint8_t ExtendedMessageIdMapping::setup_contract_signed() const { return ids[5];}
uint8_t ExtendedMessageIdMapping::setup_refund() const { return ids[6];}
uint8_t ExtendedMessageIdMapping::setup_refund_signed() const { return ids[7];}
uint8_t ExtendedMessageIdMapping::setup_contract_published() const { return ids[8];}
uint8_t ExtendedMessageIdMapping::setup_completed() const { return ids[9];}
uint8_t ExtendedMessageIdMapping::piece_get() const { return ids[10];}
uint8_t ExtendedMessageIdMapping::piece_put() const { return ids[11];}
uint8_t ExtendedMessageIdMapping::payment() const { return ids[12];}
uint8_t ExtendedMessageIdMapping::end() const { return ids[13];}
