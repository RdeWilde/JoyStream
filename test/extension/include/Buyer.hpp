/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#ifndef BUYERSESSION_HPP
#define BUYERSESSION_HPP

#include "PollableInterface.hpp"

#include <libtorrent/socket_io.hpp>

using namespace joystream;

namespace libtorrent {
    class session;
    struct add_torrent_params;
}

/**
namespace detail {
namespace buyer_torrent_management {

/// State(s)

// Sub states of `StateMachine`
struct Idle {};

// Sub states of `Started`
struct AddingTorrent {};
struct AddingTorrentFailed {};
struct StartingPlugin {};
struct StartingPluginFailed {};
struct EnteringBuyMode {};
struct EnteringBuyModeFailed {};
struct StartingDownload {};
struct StartingDownloadFailed {};
struct DownloadingStarted {

    DownloadingStarted(const protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> & info);

    protocol_session::PeerToStartDownloadInformationMap<libtorrent::tcp::endpoint> info;
};

// Sub states of `StateMachine`
struct Started {

    typedef boost::variant<AddingTorrent,
                           AddingTorrentFailed,
                           StartingPlugin,
                           StartingPluginFailed,
                           EnteringBuyMode,
                           EnteringBuyModeFailed,
                           StartingDownload,
                           StartingDownloadFailed,
                           DownloadingStarted> State;

    Started(libtorrent::session * session,
            extension::Plugin * plugin,
            const libtorrent::add_torrent_params & params,
            const protocol_wire::BuyerTerms & terms);

    State state;
    libtorrent::session * session;
    extension::Plugin * plugin;
    libtorrent::add_torrent_params params;
    protocol_wire::BuyerTerms terms;
};

struct AbstractEvent;

struct StateMachine {

    typedef boost::variant<Idle,
                           Started> State;

    StateMachine();

    void process_event(const AbstractEvent & e);

    State state;

};

/// Event(s)

/// All events must hold state machine pointer.
///
/// False reason: in order to get access to the plugin and/or session,
/// so as to send messages to them.
///
/// Real reason: above is not a good reason, as the visitor
/// will always have access to this, and can propogate
/// it, and any other required higher up state,
/// down to the subvisitors upon creation.
///
/// The real reason is that
/// it must be able to send messages to the plugin which have callbacks
/// that submit *new messages* back to the state machine, and visitors
/// dont have this information by default.

struct AbstractEvent : boost::visitor<StateMachine::State> {

    AbstractEvent(StateMachine * machine);
    virtual ~AbstractEvent() = 0;

    // Identity transitions
    virtual StateMachine::State operator()(const Idle &);
    virtual StateMachine::State operator()(const Started &);

    StateMachine * machine;
};

struct Timeout : public AbstractEvent {

    Timeout(StateMachine * machine);

    StateMachine::State operator()(const Started &);
};

struct Start : public AbstractEvent {

    Start(StateMachine * machine,
          libtorrent::session * session,
          extension::Plugin * plugin,
          const libtorrent::add_torrent_params & params,
          const protocol_wire::BuyerTerms & terms);

    StateMachine::State operator()(const Idle &);

    libtorrent::session * session;
    extension::Plugin * plugin;
    libtorrent::add_torrent_params params;
    protocol_wire::BuyerTerms terms;
};

struct TorrentPluginStatusUpdated : public AbstractEvent {

};

struct PeerPluginStatusUpdated : public AbstractEvent {

};

struct AddTorrentRequestResult : public AbstractEvent {

    AddTorrentRequestResult(StateMachine * machine,
                            const libtorrent::error_code & ec,
                            const libtorrent::torrent_handle & h);

    StateMachine::State operator()(const Started &);

    libtorrent::error_code ec;
    libtorrent::torrent_handle handle;
};

struct SubroutineRequestResult : public AbstractEvent {

    SubroutineRequestResult(StateMachine * machine,
                            const std::exception_ptr & e);

    std::exception_ptr e;
};

struct SubroutineTorrentRequestResult : public SubroutineRequestResult {

    SubroutineTorrentRequestResult(StateMachine * machine,
                                   const std::exception_ptr & e,
                                   const libtorrent::sha1_hash & info_hash);

    libtorrent::sha1_hash info_hash;
};

struct StartTorrentPluginRequestResult : public SubroutineTorrentRequestResult {

    StartTorrentPluginRequestResult(StateMachine * machine,
                                    const std::exception_ptr & e,
                                    const libtorrent::sha1_hash & info_hash);

    StateMachine::State operator()(const Started &);
};

struct ToBuyModeRequestResult : public SubroutineTorrentRequestResult {

    ToBuyModeRequestResult(StateMachine * machine,
                           const std::exception_ptr & e,
                           const libtorrent::sha1_hash & info_hash);

    StateMachine::State operator()(const Started &);
};




}
}

class Buyer : public PollableInterface {

public:

    /// All public/user triggered events

    Buyer(libtorrent::session * session);

    void buy(extension::Plugin * plugin,
             const libtorrent::add_torrent_params & params,
             const protocol_wire::BuyerTerms & terms);

    void poll();

private:

    void process(const libtorrent::alert * a);
    void process(const extension::alert::TorrentPluginStatusUpdateAlert * p);
    void process(const extension::alert::PeerPluginStatusUpdateAlert * p);

    // Needed for ::poll processing
    libtorrent::session * _session;

    detail::buyer_torrent_management::StateMachine _machine;
};
*/

/**
 * @brief Manages a session with a single torrent,
 * and, after being started, tries to initiate buying
 */
class Buyer : public PollableInterface {

public:

    Buyer(libtorrent::session * session);

    void add(const libtorrent::add_torrent_params & params);

    void connect(const libtorrent::tcp::endpoint &);

    void buy(const protocol_wire::BuyerTerms & terms);

    // void sell(); // combine with other manager here, and

    void poll();

private:

    // struct BuyerImpl


};

#endif // BUYERSESSION_HPP
