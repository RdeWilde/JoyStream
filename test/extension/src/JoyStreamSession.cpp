/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#include "JoyStreamSession.hpp"

JoyStreamSession::JoyStreamSession(const std::string & name,
                                   libtorrent::session * session,
                                   bool do_log)
    : _name(name)
    , _session(session)
    , _do_log(do_log)
    , _plugin(boost::make_shared<extension::Plugin>(1000, &_session->native_handle()->alerts(), _session->native_handle())) {

    _session->add_extension(boost::static_pointer_cast<libtorrent::plugin>(_plugin));
}

void JoyStreamSession::request_torrent_plugin_status_updates() {
    _plugin->submit(extension::request::PostTorrentPluginStatusUpdates());
}

std::vector<libtorrent::alert *> JoyStreamSession::pop_filtered_alerts() {

    std::vector<libtorrent::alert *> alerts, filtered_alerts;

    _session->pop_alerts(&alerts);

    for(auto a : alerts) {

        if(extension::alert::RequestResult const * p = libtorrent::alert_cast<extension::alert::RequestResult>(a))
            p->loadedCallback(); // Make loaded callback
        else
            filtered_alerts.push_back(a);
    }

    return filtered_alerts;
}

boost::shared_ptr<extension::Plugin> & JoyStreamSession::get_plugin() {
    return _plugin;
}

void JoyStreamSession::log(const std::string & msg) {

    if(_do_log)
        std::clog << _name  << ": " << msg << std::endl;
}
