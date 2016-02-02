/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef EXTENSION_PLUGIN_STATUS_ALERT_HPP
#define EXTENSION_PLUGIN_STATUS_ALERT_HPP

#include <libtorrent/alert.hpp>
#include <extension/alert/AlertTypes.hpp>
#include <extension/PluginStatus.hpp>

namespace joystream {
namespace extension {
namespace alert {

    class PluginStatusAlert : public libtorrent::alert {

    public:

        // Public member required for alert_cast
        const static int alert_type = PLUGIN_STATUS_ALERT_ID;

        // Default constructor
        PluginStatusAlert();

        // Constructor from members
        PluginStatusAlert(const PluginStatus & status);

        // Copy constructor
        PluginStatusAlert(const PluginStatusAlert & alert);

        // Virtual routines from libtorrent::alert
        virtual int type() const;
        virtual char const* what() const;
        virtual std::string message() const;
        virtual int category() const;
        virtual std::auto_ptr<libtorrent::alert> clone() const;

        // Getters and setters
        PluginStatus status() const;
        void setStatus(const PluginStatus & status);

    private:

        PluginStatus _status;
    };

}
}
}

#endif // EXTENSION_PLUGIN_STATUS_ALERT_HPP
