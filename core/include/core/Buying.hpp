/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 18 2016
 */

#ifndef JOYSTREAM_CORE_BUYING_HPP
#define JOYSTREAM_CORE_BUYING_HPP

#include <protocol_session/protocol_session.hpp>
#include <libtorrent/socket.hpp>

#include <QObject>

namespace joystream {
namespace core {

class Seller;

/**
 * @brief Handle for buying sub state of session
 * @note Detect expiry by listening to the Session::modeChanged signal, which
 * happens when mode changes away from buying.
 */
class Buying : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Buying(const Coin::UnspentOutputSet & funding,
           const protocol_session::BuyingPolicy & policy,
           const protocol_session::BuyingState & state,
           const protocol_wire::BuyerTerms & terms,
           const Coin::Transaction & contractTx);

    static Buying * create(const protocol_session::status::Buying<libtorrent::tcp::endpoint> & status);

    ~Buying();

    Coin::UnspentOutputSet funding() const noexcept;

    protocol_session::BuyingPolicy policy() const noexcept;

    protocol_session::BuyingState state() const noexcept;

    protocol_wire::BuyerTerms terms() const noexcept;

    std::map<libtorrent::tcp::endpoint, Seller *> sellers() const noexcept;

    Coin::Transaction contractTx() const noexcept;

signals:

    void fundingChanged(const Coin::UnspentOutputSet &);

    void policyChanged(const protocol_session::BuyingPolicy &);

    void stateChanged(const protocol_session::BuyingState &);

    void termsChanged(const protocol_wire::BuyerTerms &);

    void sellerAdded(const Seller *);

    void sellerRemoved(const libtorrent::tcp::endpoint &);

    void contractTxChanged(const Coin::Transaction &);

private:

    friend class Session;

    void addSeller(const protocol_session::status::Seller<libtorrent::tcp::endpoint> &);

    void removeSeller(const libtorrent::tcp::endpoint &);

    void removeSeller(std::map<libtorrent::tcp::endpoint, std::unique_ptr<Seller>>::iterator it);

    void update(const protocol_session::status::Buying<libtorrent::tcp::endpoint> &);

    // Funding for buyer
    Coin::UnspentOutputSet _funding;

    // Controls behaviour of session
    protocol_session::BuyingPolicy _policy;

    // State
    protocol_session::BuyingState _state;

    // Terms for buying
    protocol_wire::BuyerTerms _terms;

    // Maps connection identifier to connection
    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Seller>> _sellers;

    // Contract transaction id
    Coin::Transaction _contractTx;

    // Pieces in torrent file
    //std::vector<Piece<ConnectionIdType>> pieces;
};

}
}

#endif // JOYSTREAM_CORE_BUYING_HPP
