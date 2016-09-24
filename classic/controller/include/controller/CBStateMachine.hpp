/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 16 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_CBSTATEMACHINE_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_CBSTATEMACHINE_HPP

#include <QObject>

#include <controller/CBStateMachine.hpp>
#include <controller/Payor.hpp>
#include <controller/Payee.hpp>

namespace joystream {
namespace core {
    class CBStateMachine;
}
namespace protocol_statemachine {
    class AnnouncedModeAndTerms;
}
namespace classic {
namespace controller {

class CBStateMachine : public QObject {

    Q_OBJECT

public:

    CBStateMachine(core::CBStateMachine * CBStateMachine);

    Payor & payor() noexcept;

    Payee & payee() noexcept;

public slots:

    void setAnnouncedModeAndTermsFromPeer(const protocol_statemachine::AnnouncedModeAndTerms &);

    //void setInnerStateTypeIndex(const CBStateMachine::InnerStateIndex &);

private:

    core::CBStateMachine * _CBStateMachine;

    Payor _payor;

    Payee _payee;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_CBSTATEMACHINE_HPP
