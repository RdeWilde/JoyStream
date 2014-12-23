#include "ExtendedMessage.hpp"
#include "PassiveMessage.hpp"
#include "BuyMessage.hpp"
#include "SellMessage.hpp"

#include <libtorrent/bt_peer_connection.hpp> // bt_peer_connection::msg_extended

ExtendedMessage * ExtendedMessage::fromRaw(MessageType type, QDataStream & stream) {

    try {

        switch(type) {

            case MessageType::passive: return new PassiveMessage();
            case MessageType::buy: return new BuyMessage();
            case MessageType::end: return new SellMessage(stream);

            return NULL;
        }
    } catch (std::exception & e) {
        return NULL;
    }
}

quint32 ExtendedMessage::length() const {
    return 4 + 1 + 1 + extendedPayloadLength();
}

void ExtendedMessage::writeBEP10Header(QDataStream & stream, quint8 id) const {

    // Message length
    stream << length();

    // BEP10 message id
    stream << static_cast<quint8>(libtorrent::bt_peer_connection::msg_extended); // should be 20

    // Extended message id
    stream << id;
}
