/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 24 2016
 */

#include <controller/Seller.hpp>

namespace joystream {
namespace classic {
namespace controller {

Seller::Seller(core::Seller * seller)
    : _seller(seller) {
}

Seller::~Seller() {
}

void Seller::setState(protocol_session::SellerState sellerState) {

}

}
}
}
