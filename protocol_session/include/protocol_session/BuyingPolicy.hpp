/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, April 28 2016
 */

#ifndef JOYSTREAM_PROTOCOLSESSION_BUYINGPOLICY_HPP
#define JOYSTREAM_PROTOCOLSESSION_BUYINGPOLICY_HPP

namespace joystream {
namespace protocol_session {

class BuyingPolicy {

public:

    BuyingPolicy()
        : _minTimeBeforeBuildingContract(0)
        , _servicingPieceTimeOutLimit(0) {
    }

    BuyingPolicy(double minTimeBeforeBuildingContract, double servicingPieceTimeOutLimit)
        : _minTimeBeforeBuildingContract(minTimeBeforeBuildingContract)
        , _servicingPieceTimeOutLimit(servicingPieceTimeOutLimit) {
    }

    double minTimeBeforeBuildingContract() const {
        return _minTimeBeforeBuildingContract;
    }

    double servicingPieceTimeOutLimit() const {
        return _servicingPieceTimeOutLimit;
    }

private:

    // 1
    // The minimum amount of time (s) required before
    // trying to buil a contract
    double _minTimeBeforeBuildingContract;

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
    double _servicingPieceTimeOutLimit;

};



}
}

#endif // JOYSTREAM_PROTOCOLSESSION_BUYINGPOLICY_HPP
