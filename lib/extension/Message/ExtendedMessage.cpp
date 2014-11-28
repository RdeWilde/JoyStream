#include "extendedmessage.hpp"

ExtendedMessage::ExtendedMessage(uint8_t messageID)
    : messageID_(messageID)
{
}

uint8_t ExtendedMessage::messageIDFromType(MESSAGE_TYPES type) {

    switch(type) {

        case buy: return 0;
        case sell: return 1;
        case setup_begin: return 2;
        case setup_begin_reject: return 3;
        case setup_contract: return 4;
        case setup_contract_signed: return 5;
        case setup_refund: return 6;
        case setup_refund_signed: return 7;
        case setup_contract_published: return 8;
        case setup_completed: return 9;
        case piece_get: return 10;
        case piece_put: return 11;
        case payment: return 12;
        case end: return 13;
    }
}

ExtendedMessage::MESSAGE_TYPES ExtendedMessage::typeFromMessageID(uint8_t messageID) {

    switch(messageID) {

        case 0: return MESSAGE_TYPES::buy;
        case 1: return MESSAGE_TYPES::sell;
        case 2: return MESSAGE_TYPES::setup_begin;
        case 3: return MESSAGE_TYPES::setup_begin_reject;
        case 4: return MESSAGE_TYPES::setup_contract;
        case 5: return MESSAGE_TYPES::setup_contract_signed;
        case 6: return MESSAGE_TYPES::setup_refund;
        case 7: return MESSAGE_TYPES::setup_refund_signed;
        case 8: return MESSAGE_TYPES::setup_contract_published;
        case 9: return MESSAGE_TYPES::setup_completed;
        case 10: return MESSAGE_TYPES::piece_get;
        case 11: return MESSAGE_TYPES::piece_put;
        case 12: return MESSAGE_TYPES::payment;
        case 13: return MESSAGE_TYPES::end;
    }
}
