/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef PROTOCOL_BUY_HPP
#define PROTOCOL_BUY_HPP

#include <protocol/wire/ExtendedMessagePayload.hpp>

namespace joystream {
namespace protocol {
namespace wire {

    class Buy : public ExtendedMessagePayload {

    public:

        // Default constructor
        Buy();

        // Constructor based on members
        Buy(quint32 maxPrice, quint32 maxLock, quint32 minSellers);

        // Constructor based on raw payload
        // NB: Substitute with static factory in future, so that you cannot create stale
        // payload objects if there is an error in the reading from stream
        Buy(QDataStream & stream);

        // Virtual methods that subclassing messages have to implement
        virtual MessageType messageType() const;
        virtual quint32 length() const;
        virtual void write(QDataStream & stream) const;

        // Getters and setters
        quint32 maxPrice() const;
        void setMaxPrice(quint32 maxPrice);

        quint32 maxLock() const;
        void setMaxLock(quint32 maxLock);

        quint32 minSellers() const;
        void setMinSellers(quint32 minSellers);

    private:

        // Buyer max price (satoshies)
        quint32 _maxPrice;

        // When refund becomes spendable at latest
        quint32 _maxLock;

        // Minimum number of sellers in the contract
        quint32 _minSellers;
    };

}
}
}

#endif // PROTOCOL_BUY_HPP
