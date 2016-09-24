/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_SELLING_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_SELLING_HPP

#include <QObject>

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
}
namespace protocol_session {
    class SellingPolicy;
}
namespace core {
    class Selling;
}
namespace classic {
namespace controller {

class Selling : public QObject {

    Q_OBJECT

public:

    Selling(core::Selling * selling);

public slots:

    void setPolicy(const protocol_session::SellingPolicy &);

    void setTerms(const protocol_wire::SellerTerms &);

private:

    core::Selling * _selling;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_SELLING_HPP

