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

#include <typeindex>

namespace joystream {
namespace protocol_session {
namespace status {
    struct CBStateMachine;
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

    std::type_index innerStateTypeIndex() const noexcept;

    protocol_statemachine::AnnouncedModeAndTerms announcedModeAndTermsFromPeer() const noexcept;

    std::shared_ptr<Payor> payor() const noexcept;

    std::shared_ptr<Payee> payee() const noexcept;

signals:

    void announcedModeAndTermsFromPeerChanged(const protocol_statemachine::AnnouncedModeAndTerms &);

    void innerStateTypeIndexChanged(const std::type_index &);

private:

    friend class Connection;

    void update(const protocol_session::status::CBStateMachine &);

    std::type_index _innerStateTypeIndex;

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
