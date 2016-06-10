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

#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>

#include <extension/Status.hpp>

namespace joystream {
namespace extension {
namespace alert {

    class PluginStatusAlert : public libtorrent::alert {

    public:

        // Public member required for alert_cast
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

        // Public member required for alert_cast
        const static int alert_type = BROADCAST_TRANSACTION_ALERT_ID;

        BroadcastTransaction();

        BroadcastTransaction(const Coin::Transaction  & tx)
            : _tx(tx) { }

        BroadcastTransaction(const PluginStatusAlert & alert)
            : PluginStatusAlert(alert.tx()) { }

        // Virtual routines from libtorrent::alert
        virtual int type() const { return alert_type; }
        virtual char const* what() const { return "BroadcastTransaction"; }
        virtual std::string message() const { return std::string("BroadcastTransaction::message: IMPLEMENT LATER"); }
        virtual int category() const { return libtorrent::alert::stats_notification; }
        virtual std::auto_ptr<libtorrent::alert> clone() const { return std::auto_ptr<alert>(new PluginStatusAlert(*this)); }

        Coin::Transaction tx() const { return _tx; }

    private:

        Coin::Transaction _tx;
    };

}
}
}

#endif // JOYSTREAM_EXTENSION_ALERT_HPP
