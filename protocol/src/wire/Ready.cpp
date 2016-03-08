/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <protocol/wire/Ready.hpp>
#include <protocol/wire/MessageType.hpp>
#include <CoinCore/CoinNodeData.h> // Coin::OutPoint

namespace joystream {
namespace protocol {
namespace wire {

    Ready::Ready() {
    }

    Ready::Ready(const Coin::typesafeOutPoint & anchor)
        : _anchor(anchor) {
    }

    MessageType Ready::messageType() const {
        return MessageType::ready;
    }

    quint32 Ready::length() const {

        uchar_vector serialized = _anchor.getClassicOutPoint().getSerialized();
        return serialized.size();
    }

    void Ready::write(QDataStream & stream) const {

        uchar_vector serialized = _anchor.getClassicOutPoint().getSerialized();
        stream.writeBytes(serialized.data(), serialized.size());
    }

    Coin::typesafeOutPoint Ready::anchor() const {
        return _anchor;
    }

    void Ready::setAnchor(const Coin::typesafeOutPoint & anchor){
        _anchor = anchor;
    }
}
}
}
