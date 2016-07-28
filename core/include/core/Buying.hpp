/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#ifndef JOYSTREAM_CORE_BUYING_HPP
#define JOYSTREAM_CORE_BUYING_HPP

#include <protocol_session/protocol_session.hpp>
#include <core/Seller.hpp>
#include <libtorrent/socket.hpp>

#include <QObject>

namespace joystream {
namespace core {

class Buying : public QObject {

    Q_OBJECT

private:

    Buying(const protocol_session::status::Buying<libtorrent::tcp::endpoint> &);

public:

    Coin::UnspentP2PKHOutput funding() const noexcept;

    protocol_session::BuyingPolicy policy() const noexcept;

    protocol_session::BuyingState state() const noexcept;

    protocol_wire::BuyerTerms terms() const noexcept;

    std::map<libtorrent::tcp::endpoint, std::shared_ptr<Seller>> sellers() const noexcept;

    Coin::Transaction contractTx() const noexcept;

signals:

    void fundingChanged(const Coin::UnspentP2PKHOutput &);

    void policyChanged(const protocol_session::BuyingPolicy &);

    void stateChanged(const protocol_session::BuyingState &);

    void termsChanged(const protocol_wire::BuyerTerms &);

    void sellerAdded(const std::weak_ptr<Seller>);

    void sellerRemoved(const libtorrent::tcp::endpoint &);

    void contractTxChanged(const Coin::Transaction &);

private:

    friend class Session;

    void addSeller(const protocol_session::status::Seller<libtorrent::tcp::endpoint> &);

    void removeSeller(const libtorrent::tcp::endpoint &);

    void update(const protocol_session::status::Buying<libtorrent::tcp::endpoint> &);

    // Funding for buyer
    Coin::UnspentP2PKHOutput _funding;

    // Controls behaviour of session
    protocol_session::BuyingPolicy _policy;

    // State
    protocol_session::BuyingState _state;

    // Terms for buying
    protocol_wire::BuyerTerms _terms;

    // Maps connection identifier to connection
    std::map<libtorrent::tcp::endpoint, std::shared_ptr<Seller>> _sellers;

    // Contract transaction id
    Coin::Transaction _contractTx;

    // Pieces in torrent file
    //std::vector<Piece<ConnectionIdType>> pieces;
};

}
}

#endif // JOYSTREAM_CORE_BUYING_HPP
