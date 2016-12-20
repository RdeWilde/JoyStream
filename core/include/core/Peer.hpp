/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_PEER_HPP
#define JOYSTREAM_CORE_PEER_HPP

#include <core/PeerPlugin.hpp>

#include <libtorrent/peer_info.hpp>
#include <QObject>

#include <boost/optional.hpp>

namespace joystream {
namespace core {


/**
 * @brief Handle for peer on torrent
 * @note Detect expiry of objects by listening to the Torrent::peerRemoved
 */
class Peer : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    Peer(const libtorrent::peer_info &,
         PeerPlugin * peerPlugin);

    static Peer * create(const libtorrent::peer_info &,
                         const boost::optional<extension::status::PeerPlugin> & pluginStatus);

    ~Peer();

    libtorrent::peer_info peerInformation() const noexcept;

    /**
     * @brief Returns whether peer plugin handle is set.
     * @return whether peer plugin handle is set.
     */
    bool peerPluginSet() const noexcept;

    /**
     * @brief Returns peer plugin handle if present.
     * @throws exception::HandleNotSet if peer plugin handle is not present, i.e. !isPeerPluginSet()
     * @return peer plugin handle
     */
    PeerPlugin * peerPlugin();

signals:

    // Dropped because peer_info not compatible with Qt.
    // void peerInformationUpdated(const libtorrent::peer_info &);

    void peerPluginAdded(PeerPlugin *);

    void peerPluginRemoved();

private:

    friend struct detail::NodeImpl;
    friend class Torrent;

    void update(const libtorrent::peer_info & peerInformation);

    void addPeerPlugin(const extension::status::PeerPlugin &);

    void removePeerPlugin();

    libtorrent::peer_info _peerInformation;

    std::unique_ptr<PeerPlugin> _peerPlugin;
};

}
}

#endif // JOYSTREAM_CORE_PEER_HPP
