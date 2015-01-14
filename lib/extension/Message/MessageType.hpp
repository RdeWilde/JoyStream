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
    payment,
    end
};

#endif // MESSAGE_TYPE_HPP
