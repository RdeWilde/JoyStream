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

    PeerPlugin(const libtorrent::tcp::endpoint & endPoint,
               const extension::BEPSupportStatus & peerBEP10SupportStatus,
               const extension::BEPSupportStatus & peerBitSwaprBEPSupportStatus);

    static PeerPlugin * create(const extension::status::PeerPlugin & status);

    libtorrent::tcp::endpoint endPoint() const noexcept;

    // Indicates whether peer supports BEP10
    extension::BEPSupportStatus peerBEP10SupportStatus() const noexcept;

    // Indicates whether peer supports BEP43 .. BitSwapr
    extension::BEPSupportStatus peerBitSwaprBEPSupportStatus() const noexcept;

signals:

    void peerBEP10SupportStatusChanged(const extension::BEPSupportStatus &);

    void peerBitSwaprBEPSupportStatusChanged(const extension::BEPSupportStatus &);

private:

    friend class TorrentPlugin;

    void update(const extension::status::PeerPlugin &);

    // Endpoint: can be deduced from connection, but is worth keeping if connection pointer becomes invalid
    libtorrent::tcp::endpoint _endPoint;

    // Indicates whether peer supports BEP10
    extension::BEPSupportStatus _peerBEP10SupportStatus;

    // Indicates whether peer supports BEP43 .. BitSwapr
    extension::BEPSupportStatus _peerBitSwaprBEPSupportStatus;

};

}
}

#endif // JOYSTREAM_CORE_PEERPLUGIN_HPP
