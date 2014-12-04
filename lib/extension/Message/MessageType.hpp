#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

enum class MessageType {
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

// Number of messages
#define NUMBER_OF_MESSAGES 14

#endif // MESSAGE_TYPE_HPP
