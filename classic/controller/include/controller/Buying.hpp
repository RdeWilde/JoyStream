/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 24 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_BUYING_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_BUYING_HPP

#include <libtorrent/socket.hpp>

#include <QObject>

namespace Coin {
    class Transaction;
    class UnspentP2PKHOutput;
}

namespace joystream {
namespace protocol_wire {
    class BuyerTerms;
}
namespace protocol_session {
    class BuyingPolicy;
    enum class BuyingState;
}
namespace core {
    class Buying;
    class Seller;
}
namespace classic {
namespace controller {

class Seller;

class Buying : public QObject {

    Q_OBJECT

public:

    Buying(core::Buying * buying);

    ~Buying();

public slots:

    void setFunding(const Coin::UnspentP2PKHOutput &);

    void setPolicy(const protocol_session::BuyingPolicy &);

    void setState(const protocol_session::BuyingState &);

    void setTerms(const protocol_wire::BuyerTerms &);

    void addSeller(core::Seller *);

    void removeSeller(const libtorrent::tcp::endpoint &);

    void setContractTx(const Coin::Transaction &);

private:

    core::Buying * _buying;

    // Maps connection identifier to connection
    // NB: Has to be by ref since Seller is QObject, hence no copying
    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Seller>> _sellers;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_BUYING_HPP
