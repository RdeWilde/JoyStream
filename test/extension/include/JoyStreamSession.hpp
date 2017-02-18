/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, Feburary 18 2017
 */

#ifndef JOYSTREAMSESSION_HPP
#define JOYSTREAMSESSION_HPP

#include <libtorrent/session.hpp>
#include <extension/extension.hpp>

using namespace joystream;

/**
 * @brief A session with a joystream plugin installed
 */
class JoyStreamSession {

public:

    JoyStreamSession(const std::string & name,
                     libtorrent::session * session,
                     bool do_log);

    void request_torrent_plugin_status_updates();

    /**
     * @brief As normal libtorrent::session::pop_alerts(), only that it also dispatches extension::alert::Request results first,
     * and filters them out.
     * @return
     */
    std::vector<libtorrent::alert *> pop_filtered_alerts();

    boost::shared_ptr<extension::Plugin> & get_plugin();

private:

    std::string _name;
    libtorrent::session * _session;
    boost::shared_ptr<extension::Plugin> _plugin;
    bool _do_log;

    void log(const std::string & msg);
};


#endif // JOYSTREAMSESSION_HPP
