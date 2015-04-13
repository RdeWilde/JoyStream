#ifndef MESSAGE_TYPE_HPP
#define MESSAGE_TYPE_HPP

// Extended messages part of BEP43 wire protocol
enum class MessageType {

    observe,
    buy,
    sell,
    join_contract,
    joining_contract,
    sign_refund,
    refund_signed,
    ready,
    request_full_piece,
    full_piece,
    payment //, end
};

#endif // MESSAGE_TYPE_HPP
