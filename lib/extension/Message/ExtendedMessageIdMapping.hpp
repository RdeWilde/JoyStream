#ifndef EXTENDED_MESSAGE_ID_MAPPING_HPP
#define EXTENDED_MESSAGE_ID_MAPPING_HPP

#include <cstdint>
#include <string>

// Number of messages
#define NUMBER_OF_MESSAGES 14

class ExtendedMessageIdMapping
{

public:

    // Menu of extended message types in protocol
    enum MESSAGE_TYPE {
        buy,
        sell,
        setup_begin,
        setup_begin_reject,
        setup_contract,
        setup_contract_signed,
        setup_refund,
        setup_refund_signed,
        setup_contract_published,
        setup_completed,
        piece_get,
        piece_put,
        payment,
        end
    };

    // Default constructor. If used, then set each id individually later
    ExtendedMessageIdMapping();

    // Constructor for setting up mapping
    ExtendedMessageIdMapping(uint8_t buyId,
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
                             uint8_t endId);

    // Set extended message id of given message
    void buy(uint8_t id);
    void sell(uint8_t id);
    void setup_begin(uint8_t id);
    void setup_begin_reject(uint8_t id);
    void setup_contract(uint8_t id);
    void setup_contract_signed(uint8_t id);
    void setup_refund(uint8_t id);
    void setup_refund_signed(uint8_t id);
    void setup_contract_published(uint8_t id);
    void setup_completed(uint8_t id);
    void piece_get(uint8_t id);
    void piece_put(uint8_t id);
    void payment(uint8_t id);
    void end(uint8_t id);

    // Get extended message id of given message
    uint8_t buy() const;
    uint8_t sell() const;
    uint8_t setup_begin() const;
    uint8_t setup_begin_reject() const;
    uint8_t setup_contract() const;
    uint8_t setup_contract_signed() const;
    uint8_t setup_refund() const;
    uint8_t setup_refund_signed() const;
    uint8_t setup_contract_published() const;
    uint8_t setup_completed() const;
    uint8_t piece_get() const;
    uint8_t piece_put() const;
    uint8_t payment() const;
    uint8_t end() const;

    // Get message corresponding to id
    MESSAGE_TYPE getType(uint8_t extendedMessageId) const;

    // Get name of message
    std::string getMessageName(MESSAGE_TYPE messageType) const;

private:

    // Id of corresponding message (as seen in MESSAGE_TYPE)
    uint8_t ids[NUMBER_OF_MESSAGES];

};

#endif // EXTENDED_MESSAGE_ID_MAPPING_HPP
