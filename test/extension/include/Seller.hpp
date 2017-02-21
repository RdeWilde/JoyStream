/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#ifndef SELLERSESSIONMANAGEMENT_HPP
#define SELLERSESSIONMANAGEMENT_HPP

#include "PollableInterface.hpp"
#include <protocol_wire/protocol_wire.hpp>
#include <libtorrent/session_handle.hpp>
#include <boost/variant.hpp>

using namespace joystream;

namespace libtorrent {
    class alert;
}

/*
namespace seller_session_management {

    /// States

    struct AddingTorrent {};

    struct StartingTorrentPlugin {};

    struct EnteringSellMode {};

    struct StartingUploading {};

    struct Uploading {};

    struct Started : public PollableInterface {

        typedef boost::variant<AddingTorrent,
                               StartingTorrentPlugin,
                               EnteringSellMode,
                               StartingUploading,
                               Uploading> State;

        Started(const protocol_wire::SellerTerms & terms,
                const JoyStreamSession & session);

        virtual void poll();

        protocol_wire::SellerTerms terms;
        JoyStreamSession session;
        State _activeState;
    };

    struct Idle {};

    /// State machine

    struct StateMachine {

        typedef boost::variant<Idle, Started> State;
        typedef boost::static_visitor<State> Event;

        StateMachine();

        template<class E>
        void apply(const E & e) {
            _activeState = boost::apply_visitor(e, _activeState);
        }

        State & activeState();

    private:

        State _activeState;
    };

    /// Events

    struct Start : public StateMachine::Event {

        Start(const std::string & name,
              const protocol_wire::SellerTerms & terms,
              libtorrent::session * session,
              bool do_log = true);

        StateMachine::State operator()(const Idle & s);
        StateMachine::State operator()(const Started & s);

        std::string _name;
        protocol_wire::SellerTerms _terms;
        libtorrent::session * _session;
        bool _do_log;
    };

    struct AddTorrent {

    };

    struct StartTorrentPlugin {

    };

    struct EnterSellMode {

    };

    struct StartUpload {

    };

}
*/

#endif // SELLERSESSIONMANAGEMENT_HPP
