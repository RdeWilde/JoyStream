/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_TORRENTPLUGIN_HPP
#define JOYSTREAM_CORE_TORRENTPLUGIN_HPP

#include <extension/extension.hpp>

#include <QObject>

typedef std::map<libtorrent::tcp::endpoint, joystream::extension::status::PeerPlugin> PeerPluginStatuses;
Q_DECLARE_METATYPE(PeerPluginStatuses)

namespace joystream {
namespace core {
namespace detail {
    struct NodeImpl;
}

class PeerPlugin;

/**
 * @brief The TorrentPlugin class
 * @note Detect expiry by listening to the Torrent::torrentPluginRemoved signal
 */
class TorrentPlugin : public QObject {

    Q_OBJECT

public:

    /**
     * @brief Does MOC registration of all custome types used as signal arguments
     * on this and dependant QObjects.
     */
    static void registerMetaTypes();

    TorrentPlugin(const libtorrent::sha1_hash & infoHash,
                  const extension::status::TorrentPlugin & status,
                  const boost::shared_ptr<extension::Plugin> & plugin);

    static TorrentPlugin * create(const extension::status::TorrentPlugin & status,
                                  const boost::shared_ptr<extension::Plugin> & plugin);

    ~TorrentPlugin();

    void start(const extension::request::SubroutineHandler &);

    void stop(const extension::request::SubroutineHandler &);

    void pause(const extension::request::SubroutineHandler &);

    void updateTerms(const protocol_wire::BuyerTerms & terms,
                     const extension::request::SubroutineHandler &);

    void updateTerms(const protocol_wire::SellerTerms & terms,
                     const extension::request::SubroutineHandler &);

    void toObserveMode(const extension::request::SubroutineHandler &);

    void toSellMode(const protocol_session::GenerateP2SHKeyPairCallbackHandler & generateKeyPairCallbackHandler,
                    const protocol_session::GenerateReceiveAddressesCallbackHandler & generateReceiveAddressesCallbackHandler,
                    const protocol_session::SellingPolicy & sellingPolicy,
                    const protocol_wire::SellerTerms & terms,
                    const extension::request::SubroutineHandler &);

    void toBuyMode(const protocol_session::GenerateP2SHKeyPairCallbackHandler & generateKeyPairCallbackHandler,
                   const protocol_session::GenerateReceiveAddressesCallbackHandler & generateReceiveAddressesCallbackHandler,
                   const protocol_session::GenerateChangeAddressesCallbackHandler & generateChangeAddressesCallbackHandler,
                   const Coin::UnspentOutputSet & funding,
                   const protocol_session::BuyingPolicy & policy,
                   const protocol_wire::BuyerTerms & terms,
                   const extension::request::SubroutineHandler &);

    libtorrent::sha1_hash infoHash() const noexcept;

    extension::status::TorrentPlugin status() const noexcept;

    /**
     * @brief Requests an update of status of all corresponding peer plugins
     * on this torrent. Result is emitted on core::PeerPlugin::statusUpdated
     * signal for all known peer plugin objects know by this.
     */
    void postPeerPluginStatusUpdates() const noexcept;

signals:

    void statusUpdated(const extension::status::TorrentPlugin &);

    // Sloppy signals where we ship alerts, rahter than
    // decomposing them, not worth the effort of registering types
    // with Qt, this is all temporary.

    void anchorAnnounced(const extension::alert::AnchorAnnounced *);

    void updatePeerPluginStatuses(const PeerPluginStatuses &);

    void sessionStarted();

    void sessionPaused();

    void sessionStopped();



private:

    friend struct detail::NodeImpl;

    void update(const extension::status::TorrentPlugin &);

    /// Member fields

    libtorrent::sha1_hash _infoHash;

    extension::status::TorrentPlugin _status;

    boost::shared_ptr<extension::Plugin> _plugin;
};

}
}

#endif // JOYSTREAM_CORE_TORRENTPLUGIN_HPP
