/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_ALERT_HPP
#define JOYSTREAM_EXTENSION_ALERT_HPP

#define PLUGIN_STATUS_ALERT_ID                  (libtorrent::user_alert_id + 1)
#define REQUEST_RESULT_ALERT_ID                 (libtorrent::user_alert_id + 2)
#define ANCHOR_ANNOUNCED_ALERT_ID               (libtorrent::user_alert_id + 3)
#define BROADCAST_TRANSACTION_ALERT_ID          (libtorrent::user_alert_id + 4)

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <extension/Status.hpp>

namespace joystream {
namespace extension {
namespace request {
    struct TorrentPluginRequest;
}
namespace alert {

    struct PluginStatus : public libtorrent::alert {

        const static int alert_type = PLUGIN_STATUS_ALERT_ID;
        static const int priority = 0; // 0 = regular, 1 = high

        PluginStatus(libtorrent::aux::stack_allocator &, const status::Plugin & status)
            : status(status) {}

        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "PluginStatusAlert"; }
        virtual std::string message() const { return std::string("PluginStatusAlert"); }
        virtual int category() const { return libtorrent::alert::stats_notification; }

        status::Plugin status;
    };

    template <class T>
    struct RequestResult : public libtorrent::alert {

        const static int alert_type = REQUEST_RESULT_ALERT_ID;
        static const int priority = 0; // 0 = regular, 1 = high

        RequestResult(libtorrent::aux::stack_allocator &, const typename T::Result & result)
            : result(result) {}

        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "RequestResult"; }
        virtual std::string message() const { return std::string("RequestResult"); }
        virtual int category() const { return libtorrent::alert::error_notification; }

        typename T::Result result;
    };

    struct AnchorAnnounced : public libtorrent::torrent_alert {

        const static int alert_type = ANCHOR_ANNOUNCED_ALERT_ID;
        static const int priority = 0; // 0 = regular, 1 = high

        AnchorAnnounced(libtorrent::aux::stack_allocator & alloc,
                        libtorrent::torrent_handle const & h,
                        const libtorrent::tcp::endpoint & endPoint,
                        quint64 value,
                        const Coin::typesafeOutPoint & anchor,
                        const Coin::PublicKey & contractPk,
                        const Coin::PubKeyHash & finalPkHash)
            : libtorrent::torrent_alert(alloc, h)
            , _endPoint(endPoint)
            , _value(value)
            , _anchor(anchor)
            , _contractPk(contractPk)
            , _finalPkHash(finalPkHash) {}

        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "AnchorAnnounced"; }
        virtual std::string message() const { return std::string("AnchorAnnounced"); }
        virtual int category() const { return libtorrent::alert::error_notification; }

        libtorrent::tcp::endpoint _endPoint;
        quint64 _value;
        Coin::typesafeOutPoint _anchor;
        Coin::PublicKey _contractPk;
        Coin::PubKeyHash _finalPkHash;
    };

    struct BroadcastTransaction : public libtorrent::torrent_alert {

        const static int alert_type = BROADCAST_TRANSACTION_ALERT_ID;
        static const int priority = 0; // 0 = regular, 1 = high

        BroadcastTransaction(libtorrent::aux::stack_allocator & alloc,
                             libtorrent::torrent_handle const & h,
                             const Coin::Transaction  & tx)
            : libtorrent::torrent_alert(alloc, h)
            , tx(tx) { }

        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "BroadcastTransaction"; }
        virtual std::string message() const { return std::string("BroadcastTransaction"); }
        virtual int category() const { return libtorrent::alert::stats_notification; }

        Coin::Transaction tx;
    };
}
}
}

#endif // JOYSTREAM_EXTENSION_ALERT_HPP
