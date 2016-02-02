/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#include <extension/alert/PluginStatusAlert.hpp>

namespace joystream {
namespace extension {
namespace alert {

    PluginStatusAlert::PluginStatusAlert() {
    }

    PluginStatusAlert::PluginStatusAlert(const PluginStatus & status)
        : _status(status) {
    }

    PluginStatusAlert::PluginStatusAlert(const PluginStatusAlert & alert)
        : _status(alert.status()) {
    }

    int PluginStatusAlert::type() const {
        return alert_type;
    }

    char const* PluginStatusAlert::what() const {
        return "PluginStatusAlert";
    }

    std::string PluginStatusAlert::message() const {
        return std::string("PluginStatusAlert::message: IMPLEMENT LATER");
    }

    int PluginStatusAlert::category() const {
        return libtorrent::alert::stats_notification;
    }

    std::auto_ptr<libtorrent::alert> PluginStatusAlert::clone() const {
        return std::auto_ptr<alert>(new PluginStatusAlert(*this));
    }

    PluginStatus PluginStatusAlert::status() const {
        return _status;
    }

    void PluginStatusAlert::setStatus(const PluginStatus & status) {
        _status = status;
    }

}
}
}
