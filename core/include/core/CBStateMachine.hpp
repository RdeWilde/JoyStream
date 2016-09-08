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

#include <boost/optional.hpp>

#include <memory>
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

public:


    // Wrapped version of internal type index for the state machine.
    // We can't use raw std::type_index, as it doesn't have a defalt,
    // which Qt MOC requires
    typedef boost::optional<std::type_index> InnerStateIndex;

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    CBStateMachine(const InnerStateIndex & innerStateTypeIndex,
                   const protocol_statemachine::AnnouncedModeAndTerms & announcedModeAndTermsFromPeer,
                   Payor * payor,
                   Payee * payee);

    static CBStateMachine * create(const protocol_session::status::CBStateMachine & status);

    ~CBStateMachine();

    InnerStateIndex innerStateTypeIndex() const noexcept;

    protocol_statemachine::AnnouncedModeAndTerms announcedModeAndTermsFromPeer() const noexcept;

    Payor * payor() const noexcept;

    Payee * payee() const noexcept;

signals:

    void announcedModeAndTermsFromPeerChanged(const protocol_statemachine::AnnouncedModeAndTerms &);

    void innerStateTypeIndexChanged(const InnerStateIndex &);

private:

    friend class Connection;

    void update(const protocol_session::status::CBStateMachine &);

    InnerStateIndex _innerStateTypeIndex;

    //// Peer state
    protocol_statemachine::AnnouncedModeAndTerms _announcedModeAndTermsFromPeer;

    //// Buyer Client state
    std::unique_ptr<Payor> _payor;

    //// Seller Client state
    std::unique_ptr<Payee> _payee;

};


}
}

#endif // JOYSTREAM_CORE_CBSTATEMACHINE_HPP
