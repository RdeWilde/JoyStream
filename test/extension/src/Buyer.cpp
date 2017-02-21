/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#include "Buyer.hpp"

#include <extension/extension.hpp>

/**
namespace detail {
namespace buyer_torrent_management {

DownloadingStarted::DownloadingStarted(const protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> & info)
 : info(info) {
}

/// Started

Started::Started(libtorrent::session * session,
                 extension::Plugin * plugin,
                 const protocol_wire::BuyerTerms & terms,
                 const libtorrent::add_torrent_params & params)
    : state(AddingTorrent())
    , session(session)
    , plugin(plugin)
    , terms(terms)
    , params(params)
    , session(session) {
}

/// StateMachine

StateMachine::StateMachine()
    : state(Idle()) {
}

void StateMachine::process_event(const AbstractEvent & e) {
    state = boost::apply_visitor(e, state);
}

/// AbstractEvent

AbstractEvent::AbstractEvent(StateMachine * machine)
    : machine(machine) {
}

StateMachine::State AbstractEvent::operator()(const Idle & s) {return s;}
StateMachine::State AbstractEvent::operator()(const Started & s) {return s;}

/// Timeout

Timeout::Timeout(StateMachine *machine)
    : AbstractEvent(machine) {
}

StateMachine::State Timeout::operator()(const Started & s) {

    // We ask for status update regardless of what state we are in
    machine->session.get_plugin()->submit(extension::request::PostTorrentPluginStatusUpdates());

    return s;
}

/// Start

Start::Start(StateMachine * machine,
             libtorrent::session * session,
             extension::Plugin * plugin,
             const libtorrent::add_torrent_params & params,
             const protocol_wire::BuyerTerms & terms)
    : AbstractEvent(machine)
    , session(session)
    , plugin(plugin)
    , params(params)
    , terms(terms) {
}

StateMachine::State Start::operator()(const Idle &) {

    // Add torrent
    auto handler = [machine](libtorrent::error_code & ec, libtorrent::torrent_handle & h) {

        // Tell state machine about outcome
        machine->process_event(AddTorrentRequestResult(machine, ec, h));
    };

    plugin->submit(extension::request::AddTorrent(params, handler));

    // Transition to `Started` state
    return Started(session, plugin, params, terms);
}

// Base class for all `Started` state visitors
template<class E>
struct AbstractStartedVisitor : public boost::visitor<Started> {

    AbstractStartedVisitor(Started * parent, E * event)
        : parent(parent)
        , event(event) {
    }

    virtual ~AbstractStartedVisitor() = 0;

    /// Identity tranisitions
    virtual StateMachine::State operator()(const AddingTorrent & s) {return s;}
    virtual StateMachine::State operator()(const AddingTorrentFailed & s) {return s;}
    virtual StateMachine::State operator()(const StartingPlugin & s) { return s;}
    virtual StateMachine::State operator()(const StartingPluginFailed & s) {return s;}
    virtual StateMachine::State operator()(const EnteringBuyMode & s) {return s;}
    virtual StateMachine::State operator()(const EnteringBuyModeFailed & s) {return s;}
    virtual StateMachine::State operator()(const StartingDownload & s) {return s;}
    virtual StateMachine::State operator()(const StartingDownloadFailed & s) {return s;}
    virtual StateMachine::State operator()(const DownloadingStarted & s) {return s;}

    Started * parent;
    E * event;
};

/// AddTorrentRequestResult

AddTorrentRequestResult::AddTorrentRequestResult(StateMachine * machine,
                                                 const libtorrent::error_code & ec,
                                                 const libtorrent::torrent_handle & handle)
    : AbstractEvent(machine)
    , ec(ec)
    , handle(handle) {
}

StateMachine::State AddTorrentRequestResult::operator()(const Started & s) {

    struct StartedVisitor : public AbstractStartedVisitor<AddTorrentRequestResult> {

        StartedVisitor(Started * parent, AddTorrentRequestResult * event)
            : AbstractStartedVisitor<AddTorrentRequestResult>(parent, event) {}

        Started operator()(const AddingTorrent & s) {

            if(event->ec) {
                return AddingTorrentFailed();
            } else if(parent->params.info_hash != event->handle.info_hash()) {
                return s; // log that we ignored?
            } else {

                // Start the plugin
                auto machine = event->machine;

                auto handler = [machine](const std::exception_ptr & e) {
                    machine->process_event(StartTorrentPluginRequestResult(e,
                                                                           parent->params.info_hash));
                };

                parent->plugin->submit(extension::request::Start(parent->params.info_hash, handler));

                // Transition to `StartingPlugin` state
                return StartingPlugin();
            }
        }
    };

    StartedVisitor v(s, this);

    return boost::apply_visitor(v, s);
}

/// SubroutineRequestResult

SubroutineRequestResult::SubroutineRequestResult(StateMachine * machine,
                                                 const std::exception_ptr & e)
    : AbstractEvent(machine)
    , e(e) {
}

/// SubroutineTorrentRequestResult

SubroutineTorrentRequestResult::SubroutineTorrentRequestResult(StateMachine * machine,
                                                               const std::exception_ptr & e,
                                                               const libtorrent::sha1_hash & info_hash)
    : SubroutineRequestResult(machine, e)
    , info_hash(info_hash) {
}

/// StartTorrentPluginRequestResult

StartTorrentPluginRequestResult::StartTorrentPluginRequestResult(StateMachine * machine,
                                                                 const std::exception_ptr & e,
                                                                 const libtorrent::sha1_hash & info_hash)
    : SubroutineTorrentRequestResult(machine, e, info_hash) {
}

StateMachine::State StartTorrentPluginRequestResult::operator()(const Started &) {

    struct StartedVisitor : public AbstractStartedVisitor<AddTorrentRequestResult> {

        StartedVisitor(Started * parent, StartTorrentPluginRequestResult * event)
            : AbstractStartedVisitor<StartTorrentPluginRequestResult>(parent, event) {}

        Started operator()(const StartingPlugin & s) {

            if(event->e) {
                return StartingPluginFailed();
            } else if(parent->params.info_hash != event->info_hash) {
                return s; // log that we ignored?
            } else {

                // Go to buy mode
                auto machine = event->machine;

                auto handler = [machine](const std::exception_ptr & e) {
                    machine->process_event(ToBuyModeRequestResult(e));
                };

                parent->plugin->submit(extension::request::ToBuyMode(parent->params.info_hash,
                                                                     parent->terms,
                                                                     handler));

                // Transition to `EnteringBuyMode` state
                return EnteringBuyMode();
            }
        }
    };

    StartedVisitor v(s, this);

    return boost::apply_visitor(v, s);
}

/// ToBuyModeRequestResult


ToBuyModeRequestResult::ToBuyModeRequestResult(StateMachine * machine, const std::exception_ptr & e, const libtorrent::sha1_hash & info_hash)
    : SubroutineTorrentRequestResult(machine, e, info_hash) {
}

StateMachine::State ToBuyModeRequestResult::operator()(const Started &) {

    struct StartedVisitor : public AbstractStartedVisitor<AddTorrentRequestResult> {

        StartedVisitor(Started * parent, StartTorrentPluginRequestResult * event)
            : AbstractStartedVisitor<StartTorrentPluginRequestResult>(parent, event) {}

        Started operator()(const StartingPlugin & s) {

            if(event->e) {
                return StartingPluginFailed();
            } else if(parent->params.info_hash != event->info_hash) {
                return s; // log that we ignored?
            } else {

                // Go to buy mode
                auto machine = event->machine;

                auto handler = [machine](const std::exception_ptr & e) {
                    machine->process_event(ToBuyModeRequestResult(e));
                };

                parent->plugin->submit(extension::request::ToBuyMode(parent->params.info_hash,
                                                                     parent->terms,
                                                                     handler));

                // Transition to `EnteringBuyMode` state
                return EnteringBuyMode();
            }
        }
    };

    StartedVisitor v(s, this);

    return boost::apply_visitor(v, s);
}

}
}
*/

////////

/**
std::map<libtorrent::tcp::endpoint, protocol_wire::SellerTerms> select_N_sellers(unsigned int N, const std::map<libtorrent::tcp::endpoint, protocol_session::status::PeerPlugin> & statuses) {

    std::map<libtorrent::tcp::endpoint, protocol_wire::SellerTerms> selected;

    for(auto s : statuses) {

       libtorrent::tcp::endpoint ep = s.first;
       extension::status::PeerPlugin status = s.second;

       if(status.peerBEP10SupportStatus == extension::BEPSupportStatus::supported &&
          status.peerBitSwaprBEPSupportStatus == extension::BEPSupportStatus::supported &&
          status.connection.is_initialized()) {

           const protocol_session::status::Connection<libtorrent::tcp::endpoint> & connection_status = status.connection.get();

           if(connection_status.machine.innerStateTypeIndex == typeid(protocol_statemachine::PreparingContract)) {

               selected.insert(std::make_pair(ep, ))

           }
       }
    }
}

Coin::Transaction make_contract_tx(const std::map<libtorrent::tcp::endpoint, protocol_wire::SellerTerms> & sellers) {

}

protocol_session::StartDownloadConnectionInformation make_connection_information(const protocol_wire::SellerTerms & terms) {

    return protocol_session::StartDownloadConnectionInformation inf(0, 10000,);

    // Terms which was the basis for the contract initation
    protocol_wire::SellerTerms sellerTerms;

    // Contract output index
    uint32_t index;

    // Contract output value
    int64_t value;

    /// Buyer

    // Contract output buyer multisig key
    Coin::KeyPair buyerContractKeyPair;

    // Payment/Refund buyer output
    Coin::PubKeyHash buyerFinalPkHash;

}


Buyer::Buyer(libtorrent::session * session)
    : _session(session) {
}

void Buyer::buy(extension::Plugin * plugin,
                const libtorrent::add_torrent_params & params,
                const protocol_wire::BuyerTerms & terms) {

    _machine.process_event(detail::buyer_torrent_management::Start(&_machine,
                                                                   session,
                                                                   plugin,
                                                                   params,
                                                                   terms));
}

void Buyer::poll() {

    /// The purpose of this routine is to facilitate communication between
    /// the state machine and the session/plugin.

    /// a) To get messages from the latter to the former, session::pop_alerts
    /// is used, and extension::alert::RequestResult are also processed, as they
    /// can be used to send messages to the state machine also

    // Process alerts
    std::vector<libtorrent::alert *> alerts;
    auto alerts = _session->pop_alerts(&alerts);

    for(auto a : alerts) {

        if(extension::alert::RequestResult const * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
            p->loadedCallback(); // Make loaded callback
        else
            process(a);
    }

    /// b) To allow former to ask latter for messages with state updates,
    /// a timeout event is submitted to it.

    // Submit timeout event
    _machine.process_event(detail::buyer_torrent_management::Timeout(this));
}

void Buyer::process(const libtorrent::alert * a) {

    if(extension::alert::TorrentPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::TorrentPluginStatusUpdateAlert>(a))
        process(p);
    else if(extension::alert::PeerPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::PeerPluginStatusUpdateAlert>(a))
        process(p);

    // ** if peer disconnect occured, then that is a problem? **
}

void Buyer::process(const extension::alert::TorrentPluginStatusUpdateAlert * p) {

    for(auto m: p->statuses)
        session.plugin->submit(extension::request::PostPeerPluginStatusUpdates(m.first));
}

void Buyer::process(const extension::alert::PeerPluginStatusUpdateAlert * p) {

    if(state == State::buy_mode_started) {

        //

        std::map<libtorrent::tcp::endpoint, protocol_wire::SellerTerms> sellers;


        try {
            sellers = selected_N_sellers(terms.minNumberOfSellers(), p->statuses);
        } catch(const std::runtime_error & e) {
            log("....");
            return;
        }

        // Create contract transaction
        Coin::Transaction tx = make_contract_tx(sellers);

        protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> map;

        assert(!startDownloadingInformation.is_initialized());
        protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> map;

        startDownloadingInformation = map;

        session.plugin->submit(extension::request::StartDownloading(infoHash, contractTx, peerToStartDownloadInformationMap,
                                                                    [=](const std::exception_ptr & e) -> void {

            if(e) {
                state = State::downloading_starting_failed;
                return;
            } else
                state = State::downloading_started;

            // we are done, nothing more to do

        }));

    }
}
*/

/// Buyer

Buyer::Buyer()
    : _state(Idle()) {
}

void Buyer::start(libtorrent::session * session,
                  extension::Plugin * plugin,
                  const libtorrent::add_torrent_params & params,
                  const protocol_wire::BuyerTerms & terms) {

    if(Idle * s = boost::get<Idle>(&_state))
        _state = Started(session, plugin, params,terms);
    else
        throw std::runtime_error("Cannot start, is already started.");
}

void Buyer::poll() {

    /// The purpose of this routine is to facilitate communication between
    /// the state machine and the session/plugin.
    ///
    /// a) To get messages from the latter to the former, session::pop_alerts
    /// is used, and extension::alert::RequestResult are also processed, as they
    /// can be used to send messages to the state machine also
    ///
    /// b) To allow former to ask latter for messages with state updates,
    /// a timeout event is submitted to it.

    if(Started * s = boost::get<Started>(&_state)) {

        // Process alerts
        std::vector<libtorrent::alert *> alerts;
        s->session->pop_alerts(&alerts);

        for(auto a : alerts) {

            if(extension::alert::RequestResult const * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
                p->loadedCallback(); // Make loaded callback
            else
                s->process(a);
        }

        // Get status update on torrent plugins
        s->plugin->submit(extension::request::PostTorrentPluginStatusUpdates());
    }
}

/// Buyer::Started

Buyer::Started::Started(libtorrent::session * session,
                        extension::Plugin * plugin,
                        const libtorrent::add_torrent_params & params,
                        const protocol_wire::BuyerTerms & terms)
    : state(State::adding_torrent)
    , session(session)
    , plugin(plugin)
    , params(params)
    , terms(terms) {

    // Add torrent
    plugin->submit(extension::request::AddTorrent(params, [this, plugin, params, terms](libtorrent::error_code & ec, libtorrent::torrent_handle &) -> void {

        if(ec) {
            state = State::torrent_adding_failed;
            return;
        }

        state = State::starting_plugin;

        // Start plugin
        plugin->submit(extension::request::Start(params.info_hash, [this, plugin, params, terms](const std::exception_ptr & e) {

            if(e) {
                state = State::plugin_starting_failed;
                return;
            }

            state = State::starting_buy_mode;

            // To buy mode
            plugin->submit(extension::request::ToBuyMode(params.info_hash, terms, [this, plugin, params, terms](const std::exception_ptr & e) {

                if(e) {
                    state = State::buy_mode_starting_failed;
                    return;
                }

                state = State::buy_mode_started;

                // At this point, we have to wait for an asynchronous event, namely
                // that a connection with a suitable seller is estalished,
                // and we catch this in alert processor.

            }));
        }));
    }));

}

void Buyer::Started::process(const libtorrent::alert * a) {

    if(extension::alert::TorrentPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::TorrentPluginStatusUpdateAlert>(a))
        process(p);
    else if(extension::alert::PeerPluginStatusUpdateAlert const * p = libtorrent::alert_cast<extension::alert::PeerPluginStatusUpdateAlert>(a))
        process(p);

    // ** if peer disconnect occured, then that is a problem? **
}

void Buyer::Started::process(const extension::alert::TorrentPluginStatusUpdateAlert * p) {

    for(auto m: p->statuses)
        plugin->submit(extension::request::PostPeerPluginStatusUpdates(m.first));
}

struct SellerInformation {

    SellerInformation() {}

    SellerInformation(const protocol_wire::SellerTerms & terms,
                      const Coin::PublicKey & contractPk)
        : terms(terms)
        , contractPk(contractPk) {
    }

    protocol_wire::SellerTerms terms;
    Coin::PublicKey contractPk;
};

std::map<libtorrent::tcp::endpoint, SellerInformation> select_N_sellers(unsigned int N,
                                                                        const std::map<libtorrent::tcp::endpoint, extension::status::PeerPlugin> & statuses);
//Coin::Transaction make_contract_tx(const protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> & map);

void Buyer::Started::process(const extension::alert::PeerPluginStatusUpdateAlert * p) {

    if(state == State::buy_mode_started) {

        std::map<libtorrent::tcp::endpoint, SellerInformation> sellers;

        try {
            sellers = select_N_sellers(terms.minNumberOfSellers(), p->statuses);
        } catch(const std::runtime_error & e) {
            //log("Coulndt find sufficient number of suitable sellers");
            return;
        }

        // Create contract commitments and download information
        protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> map;
        paymentchannel::ContractTransactionBuilder::Commitments commitments(sellers.size());

        uint32_t output_index = 0;
        for(const auto & s: sellers) {

            // fixed for now
            int64_t value = 100000;
            Coin::KeyPair buyerKeyPair(Coin::PrivateKey::generate());  // **replace later with determinisic key**
            Coin::PubKeyHash buyerFinalPkHash;

            protocol_session::StartDownloadConnectionInformation inf(s.second.terms,
                                                                                output_index,
                                                                                value,
                                                                                buyerKeyPair,
                                                                                buyerFinalPkHash);

            map.insert(std::make_pair(s.first, inf));


            commitments[output_index] = paymentchannel::Commitment(value,
                                                                   buyerKeyPair.pk(),
                                                                   s.second.contractPk, // payeePK
                                                                   Coin::RelativeLockTime(Coin::RelativeLockTime::Units::Time, s.second.terms.minLock()));

            output_index++;
        }

        // Create contract transaction
        paymentchannel::ContractTransactionBuilder c;
        c.setCommitments(commitments);

        Coin::Transaction tx = c.transaction();

        plugin->submit(extension::request::StartDownloading(p->handle.info_hash(), tx, map, [=](const std::exception_ptr & e) -> void {

            if(e) {
                state = State::downloading_starting_failed;
                //log("....");
                return;
            } else
                state = State::downloading_started;

            // we are done, nothing more to do?

        }));

    }
}

std::map<libtorrent::tcp::endpoint, SellerInformation> select_N_sellers(unsigned int N, const std::map<libtorrent::tcp::endpoint, extension::status::PeerPlugin> & statuses) {

    std::map<libtorrent::tcp::endpoint, SellerInformation> selected;

    for(auto s : statuses) {

       libtorrent::tcp::endpoint ep = s.first;
       extension::status::PeerPlugin status = s.second;

       if(status.peerBEP10SupportStatus == extension::BEPSupportStatus::supported &&
          status.peerBitSwaprBEPSupportStatus == extension::BEPSupportStatus::supported &&
          status.connection.is_initialized()) {

           auto & machine = status.connection.get().machine;

           if(machine.innerStateTypeIndex == typeid(protocol_statemachine::PreparingContract) && selected.size() < N)
               selected[ep] = SellerInformation(machine.announcedModeAndTermsFromPeer.sellModeTerms(),
                                                machine.payor.payeeContractPk());
       }
    }

    return selected;
}
