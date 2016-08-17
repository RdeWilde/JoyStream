/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 16 2016
 */

#ifndef JOYSTREAM_CORE_CBSTATEMACHINE_HPP
#define JOYSTREAM_CORE_CBSTATEMACHINE_HPP

#include <protocol_statemachine/protocol_statemachine.hpp>

#include <QObject>

namespace joystream {
namespace protocol_session {
namespace status {
    class CBStateMachine;
}
}
namespace core {

class Payor;
class Payee;

class CBStateMachine : public QObject {

    Q_OBJECT

private:

    CBStateMachine(const protocol_session::status::CBStateMachine &);

public:

    protocol_statemachine::AnnouncedModeAndTerms announcedModeAndTermsFromPeer() const noexcept;

    std::shared_ptr<Payor> payor() const noexcept;

    std::shared_ptr<Payee> payee() const noexcept;

signals:

    void announcedModeAndTermsFromPeerChanged(const protocol_statemachine::AnnouncedModeAndTerms &);

private:

    friend class Connection;

    void update(const protocol_session::status::CBStateMachine &);

    //// Peer state
    protocol_statemachine::AnnouncedModeAndTerms _announcedModeAndTermsFromPeer;

    //// Buyer Client state
    std::shared_ptr<Payor> _payor;

    //// Seller Client state
    std::shared_ptr<Payee> _payee;

};


}
}

#endif // JOYSTREAM_CORE_CBSTATEMACHINE_HPP
