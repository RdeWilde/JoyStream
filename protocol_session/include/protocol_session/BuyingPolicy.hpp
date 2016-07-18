/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 28 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_BUYINGPOLICY_HPP
#define JOYSTREAM_PROTOCOLSESSION_BUYINGPOLICY_HPP

#include <protocol_wire/protocol_wire.hpp>
#include <chrono>

namespace joystream {
namespace protocol_session {

class BuyingPolicy {

public:

    BuyingPolicy()
        : _minTimeBeforeBuildingContract(0)
        , _servicingPieceTimeOutLimit(0)
        , _sellerTermsOrderingPolicy(protocol_wire::SellerTerms::OrderingPolicy::min_price) // fixed, but arbitary, default value
        {
    }

    BuyingPolicy(double minTimeBeforeBuildingContract, double servicingPieceTimeOutLimit, protocol_wire::SellerTerms::OrderingPolicy policy)
        : _minTimeBeforeBuildingContract(minTimeBeforeBuildingContract)
        , _servicingPieceTimeOutLimit(servicingPieceTimeOutLimit)
        , _sellerTermsOrderingPolicy(policy) {
    }

    bool operator==(const BuyingPolicy & rhs) const {
        return _minTimeBeforeBuildingContract == rhs.minTimeBeforeBuildingContract() &&
               _servicingPieceTimeOutLimit == rhs.servicingPieceTimeOutLimit() &&
               _sellerTermsOrderingPolicy == rhs.sellerTermsOrderingPolicy();
    }

    std::chrono::duration<double> minTimeBeforeBuildingContract() const {
        return _minTimeBeforeBuildingContract;
    }

    std::chrono::duration<double> servicingPieceTimeOutLimit() const {
        return _servicingPieceTimeOutLimit;
    }

    protocol_wire::SellerTerms::OrderingPolicy sellerTermsOrderingPolicy() const {
        return _sellerTermsOrderingPolicy;
    }

private:

    // 1
    // The minimum amount of time (s) required before
    // trying to buil a contract
    std::chrono::duration<double> _minTimeBeforeBuildingContract;

    // 2
    // time to wait, at very least,

    // 3
    // How long to wait before trying to ask someone else for the same piece
    // if a peer is not responding, or responding too slowly

    // 4
    // Ranking peers for invite:
    // if there are more peers than your desired max,
    // do you pick random subset, or minimize on price.

    // 5
    // Ranking peers for downloading

    // 6
    // Ranking piece for download order

    // 7
    //
    std::chrono::duration<double> _servicingPieceTimeOutLimit;


    // 8
    // How sellers should be ranked in terms of their terms
    protocol_wire::SellerTerms::OrderingPolicy _sellerTermsOrderingPolicy;

};

}
}

#endif // JOYSTREAM_PROTOCOLSESSION_BUYINGPOLICY_HPP
