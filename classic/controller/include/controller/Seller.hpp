/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 24 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_SELLER_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_SELLER_HPP

#include <QObject>

namespace joystream {
namespace core {
    class Seller;
}
namespace protocol_session {
    enum class SellerState;
}
namespace classic {
namespace controller {

class Seller : public QObject {

    Q_OBJECT

public:

    Seller(core::Seller * seller);

    ~Seller();

public slots:

    void setState(protocol_session::SellerState);

private:

    core::Seller * _seller;

};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_SELLER_HPP

