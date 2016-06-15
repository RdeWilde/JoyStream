/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 7 2016
 */

#ifndef JOYSTREAM_EXTENSION_ALERT_HPP
#define JOYSTREAM_EXTENSION_ALERT_HPP

#define PLUGIN_STATUS_ALERT_ID                  (libtorrent::user_alert_id + 1)
#define BROADCAST_TRANSACTION_ALERT_ID          (libtorrent::user_alert_id + 2)
#define REQUEST_RESULT_ALERT_ID                 (libtorrent::user_alert_id + 3)
#define ANCHOR_ANNOUNCED_ALERT_ID               (libtorrent::user_alert_id + 4)

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <extension/Status.hpp>

namespace joystream {
namespace extension {
namespace alert {

    // Rule: Alerts are used for operations which have no return value,
    // and where thread running libtorrent alert loop is client, and hence
    // we get synchornization for free.

    class PluginStatusAlert : public libtorrent::alert {

    public:

        const static int alert_type = PLUGIN_STATUS_ALERT_ID;

        PluginStatusAlert();

        PluginStatusAlert(const status::Plugin & status)
            : _status(status) { }

        PluginStatusAlert(const PluginStatusAlert & alert)
            : PluginStatusAlert(alert.status()) { }

        // Virtual routines from libtorrent::alert
        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "PluginStatusAlert"; }
        virtual std::string message() const { return std::string("PluginStatusAlert::message: IMPLEMENT LATER"); }
        virtual int category() const { return libtorrent::alert::stats_notification; }
        virtual std::auto_ptr<libtorrent::alert> clone() const { return std::auto_ptr<alert>(new PluginStatusAlert(*this)); }


        status::Plugin status() const { return _status; }

    private:

        status::Plugin _status;
    };

    class BroadcastTransaction : public libtorrent::alert {

    public:

        const static int alert_type = BROADCAST_TRANSACTION_ALERT_ID;

        BroadcastTransaction();

        BroadcastTransaction(const Coin::Transaction  & tx)
            : _tx(tx) { }

        BroadcastTransaction(const BroadcastTransaction & alert)
            : BroadcastTransaction(alert.tx()) { }

        // Virtual routines from libtorrent::alert
        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "BroadcastTransaction"; }
        virtual std::string message() const { return std::string("BroadcastTransaction::message: IMPLEMENT LATER"); }
        virtual int category() const { return libtorrent::alert::stats_notification; }
        virtual std::auto_ptr<libtorrent::alert> clone() const { return std::auto_ptr<alert>(new BroadcastTransaction(*this)); }

        Coin::Transaction tx() const { return _tx; }

    private:

        Coin::Transaction _tx;
    };

    template <class R>
    class RequestResult : public libtorrent::alert {

    public:

        const static int alert_type = REQUEST_RESULT_ALERT_ID;

        RequestResult();

        RequestResult(const R & result)
            : _result(result) {}

        RequestResult(const RequestResult & alert)
            : RequestResult(alert.result()) { }

        // Virtual routines from libtorrent::alert
        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "RequestResult"; }
        virtual std::string message() const { return std::string("RequestResult::message: IMPLEMENT LATER"); }
        virtual int category() const { return libtorrent::alert::error_notification; }
        virtual std::auto_ptr<libtorrent::alert> clone() const { return std::auto_ptr<alert>(new RequestResult(*this)); }

        R result() const { return _result; }

    private:

        // Request result
        R _result;
    };

    class AnchorAnnounced : public libtorrent::alert {

    public:

        const static int alert_type = ANCHOR_ANNOUNCED_ALERT_ID;

        AnchorAnnounced();

        AnchorAnnounced(const libtorrent::tcp::endpoint & endPoint,
                        quint64 value,
                        const Coin::typesafeOutPoint & anchor,
                        const Coin::PublicKey & contractPk,
                        const Coin::PubKeyHash & finalPkHash)
            : _endPoint(endPoint)
            , _value(value)
            , _anchor(anchor)
            , _contractPk(contractPk)
            , _finalPkHash(finalPkHash) {}

        AnchorAnnounced(const AnchorAnnounced & alert)
            : AnchorAnnounced(alert.endPoint(),
                              alert.value(),
                              alert.anchor(),
                              alert.contractPk(),
                              alert.finalPkHash()) {}

        // Virtual routines from libtorrent::alert
        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "AnchorAnnounced"; }
        virtual std::string message() const { return std::string("AnchorAnnounced::message: IMPLEMENT LATER"); }
        virtual int category() const { return libtorrent::alert::error_notification; }
        virtual std::auto_ptr<libtorrent::alert> clone() const { return std::auto_ptr<alert>(new AnchorAnnounced(*this)); }

        libtorrent::tcp::endpoint endPoint() const { return _endPoint; }
        quint64 value() const { return _value; }
        Coin::typesafeOutPoint anchor() const { return _anchor; }
        Coin::PublicKey contractPk() const { return _contractPk; }
        Coin::PubKeyHash finalPkHash() const { return _finalPkHash; }

    private:

        libtorrent::tcp::endpoint _endPoint;
        quint64 _value;
        Coin::typesafeOutPoint _anchor;
        Coin::PublicKey _contractPk;
        Coin::PubKeyHash _finalPkHash;

    };

}
}
}

#endif // JOYSTREAM_EXTENSION_ALERT_HPP