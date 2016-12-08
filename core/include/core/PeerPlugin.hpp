/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_PEERPLUGIN_HPP
#define JOYSTREAM_CORE_PEERPLUGIN_HPP

#include <extension/extension.hpp>

#include <QObject>

namespace joystream {
namespace core {
namespace detail {
    struct NodeImpl;
}

/**
 * @brief Handle for peer plugin on torrent plugin
 * @note Detect expiry by listening to the TorrentPlugin::peerPluginRemoved
 */
class PeerPlugin : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    PeerPlugin(const extension::status::PeerPlugin & status);

    ~PeerPlugin();

    extension::status::PeerPlugin status() const noexcept;

signals:

    void statusUpdated(const extension::status::PeerPlugin &);

    void connectionAdded(const protocol_session::status::Connection<libtorrent::tcp::endpoint> & status);

    void connectionRemoved();

private:

    friend struct detail::NodeImpl;

    void update(const extension::status::PeerPlugin &);

    extension::status::PeerPlugin _status;
};

}
}

#endif // JOYSTREAM_CORE_PEERPLUGIN_HPP
