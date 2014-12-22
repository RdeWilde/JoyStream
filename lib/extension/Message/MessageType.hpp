#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

// Extended messages part of BEP43 wire protocol
enum class MessageType {

    // First message is one of these
    passive,
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


#endif // MESSAGE_TYPE_HPP
