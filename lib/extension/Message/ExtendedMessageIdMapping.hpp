#ifndef EXTENDED_MESSAGE_ID_MAPPING_HPP
#define EXTENDED_MESSAGE_ID_MAPPING_HPP

#include "MessageType.hpp"

#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/entry.hpp>

#include <QDataStream> // for data types
#include <string>

class ExtendedMessageIdMapping
{

public:

    // Default constructor. If used, then set each id individually later
    ExtendedMessageIdMapping();

    // Constructor from extended hanshake dictionary m, used when parsing hanshake from peer
    ExtendedMessageIdMapping(const libtorrent::lazy_entry * m);

    // Constructor for setting up mapping
    ExtendedMessageIdMapping(quint8 buyId,
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
                             quint8 endId);

    // Outputs mapping into and m dicitonary, which can be used in extended handshake
    void writeToDictionary(libtorrent::entry::dictionary_type & m);

    // Set extended message id of given message
    void buy(quint8 id);
    void sell(quint8 id);
    void setup_begin(quint8 id);
    void setup_begin_reject(quint8 id);
    void setup_contract(quint8 id);
    void setup_contract_signed(quint8 id);
    void setup_refund(quint8 id);
    void setup_refund_signed(quint8 id);
    void setup_contract_published(quint8 id);
    void setup_completed(quint8 id);
    void piece_get(quint8 id);
    void piece_put(quint8 id);
    void payment(quint8 id);
    void end(quint8 id);

    // Get extended message id of given message
    quint8 buy() const;
    quint8 sell() const;
    quint8 setup_begin() const;
    quint8 setup_begin_reject() const;
    quint8 setup_contract() const;
    quint8 setup_contract_signed() const;
    quint8 setup_refund() const;
    quint8 setup_refund_signed() const;
    quint8 setup_contract_published() const;
    quint8 setup_completed() const;
    quint8 piece_get() const;
    quint8 piece_put() const;
    quint8 payment() const;
    quint8 end() const;

    // Get message corresponding to id
    MessageType getMessageType(quint8 id) const;

    // Get message corresponding to message name
    static MessageType getMessageType(std::string & messageName);

    // Get name of message
    static std::string getMessageName(MessageType type);

    // Check that mapping is valid
    bool isValid() const;

    // Sets all message ids starting at the given parameter
    void setAllStartingAt(quint8 id);

private:

    // Id of corresponding message
    quint8 buyId_,
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
            endId_;

};

#endif // EXTENDED_MESSAGE_ID_MAPPING_HPP
