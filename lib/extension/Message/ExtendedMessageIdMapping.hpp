#ifndef EXTENDED_MESSAGE_ID_MAPPING_HPP
#define EXTENDED_MESSAGE_ID_MAPPING_HPP

#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/entry.hpp>

#include <QDataStream> // for data types
#include <string>

namespace Message {

    // Menu of extended message types in protocol,
    // and value is array index in array "ids" where
    // id of corresponding message is kept
    // CRITICAL: Code assumes enum values start at 0 increase with one
    enum TYPE {
        buy = 0,
        sell = 1,
        setup_begin = 2,
        setup_begin_reject = 3,
        setup_contract = 4,
        setup_contract_signed = 5,
        setup_refund = 6,
        setup_refund_signed = 7,
        setup_contract_published = 8,
        setup_completed = 9,
        piece_get = 10,
        piece_put = 11,
        payment = 12,
        end = 13
    };

    // Number of messages
    const static int NUMBER_OF_MESSAGES = TYPE::end + 1 ;
}

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
    Message::TYPE getMessageType(quint8 id) const;

    // Get message corresponding to message name
    static Message::TYPE getMessageType(std::string & messageName);

    // Get name of message
    static std::string getMessageName(Message::TYPE type);

    // Check that mapping is valid
    bool isValid() const;

    // Sets all message ids starting at the given parameter
    void setAllStartingAt(quint8 id);

private:

    // Id of corresponding message (as seen in MESSAGE_TYPE)
    quint8 ids[Message::NUMBER_OF_MESSAGES];

};

#endif // EXTENDED_MESSAGE_ID_MAPPING_HPP
