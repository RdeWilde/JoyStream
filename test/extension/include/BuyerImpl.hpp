/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#ifndef BUYERIMPL_HPP
#define BUYERIMPL_HPP

#include <protocol_wire/protocol_wire.hpp>
#include <protocol_session/protocol_session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <boost/variant.hpp>

using namespace joystream;


namespace joystream {
namespace extension {
    class Plugin;
namespace alert {
    struct TorrentPluginStatusUpdateAlert;
    struct PeerPluginStatusUpdateAlert;
}
}
}

struct Init {};
struct AddedTorrent {

    struct Init {};

    struct StartingPlugin {};
    struct StartingPluginFailed {};

    struct Buy {

        struct StartingBuyMode {};
        struct StartingBuyModeFailed {};

        struct StartingDownload {};
        struct StartingDownloadFailed {};

        struct DownloadingStartingFailed {};
        struct DownloadingStarted {};

        typedef boost::variant<StartingBuyMode,
                               StartingBuyModeFailed,
                               StartingDownload,
                               StartingDownloadFailed,
                               // Moving on beyond this state requires suitable alert state
                               DownloadingStartingFailed,
                               DownloadingStarted> State;

        Buy(const protocol_wire::BuyerTerms terms);

        protocol_wire::BuyerTerms terms;
    };

    struct Sell {

    };

    struct Observe {

    };

    typedef boost::variant<Init,
                           StartingPlugin,
                           StartingPluginFailed,
                           Buy,
                           Sell,
                           Observe> State;

    State state;
    libtorrent::add_torrent_params params;
};

struct BuyerImpl {

    typedef boost::variant<Init,
                           AddedTorrent> State;

    void process(const libtorrent::alert * a);
    void process(const extension::alert::TorrentPluginStatusUpdateAlert * p);
    void process(const extension::alert::PeerPluginStatusUpdateAlert * p);

    State state;
    libtorrent::session * session;
    boost::shared_ptr<extension::Plugin> plugin;

}

#endif // BUYERIMPL_HPP
