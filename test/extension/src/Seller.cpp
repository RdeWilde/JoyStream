/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#include "Seller.hpp"

/**
namespace seller_session_management {

// Started

Started::Started(const protocol_wire::SellerTerms & terms,
                 const JoyStreamSession & session)
    : terms(terms)
    , session(session) {
}

void Started::poll() {

    session.request_torrent_plugin_status_updates();

    // Alert processing
    std::vector<libtorrent::alert *> alerts = session.pop_filtered_alerts();

    /// !!!! process alerts !!!!

}

// Start

Start::Start(const std::string & name,
             const protocol_wire::SellerTerms & terms,
             libtorrent::session * session,
             bool do_log)
    : _name(name)
    , _terms(terms)
    , _session(session)
    , _do_log(do_log) {
}

StateMachine::State Start::operator()(const Idle &) {


    // - add torrent on both
    // - only proceed its live on both
    // - wait until its in seed mode on seller side?

    return new_state;
}

StateMachine::State Start::operator()(const Started & s) {
    return s;
}

// StateMachine

StateMachine::StateMachine()
    : _activeState(Idle()) {
}

StateMachine::State & StateMachine::activeState() {
    return _activeState;
}

}
*/

/**
// Create new state
// 14++ for std::make_unique
Started new_state(_terms, JoyStreamSession(_name, _session, _do_log));

// - add torrent
new_state.session.get_plugin()->submit(extension::request::AddTorrent(params,
                                                                      [=](libtorrent::error_code & ec, libtorrent::torrent_handle & h) -> void {

    if(ec) {
        // assert false
        return;
    } else
        state = State::torrent_added;


    // Start plugin
    session.plugin->submit(extension::request::Start(h.info_hash(),
                                                     [=](const std::exception_ptr & e) {

        if(e) {
            state = State::plugin_starting_failed;
            return;
        } else
            state = State::plugin_started;

        // To buy mode
        session.plugin->submit(extension::request::ToBuyMode(h.info_hash(), terms, [=](const std::exception_ptr & e) {

            if(e) {
                state = State::buy_mode_starting_failed;
                return;
            } else
                state = State::buy_mode_started;

            // At this point, we have to wait for an asynchronous event, namely
            // that a connection with a suitable seller is estalished,
            // and we catch this in alert processor.

        }));
    }));
}));
*/
