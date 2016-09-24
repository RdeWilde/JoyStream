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

namespace joystream {
namespace core {

class PeerPlugin;
class Session;

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
                  Session * session,
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

    /**
     * @brief Returns peer plugin object mapping
     * @return mapping of endpoint to peer plugin object
     */
    std::map<libtorrent::tcp::endpoint, PeerPlugin *> peers() const noexcept;

    /**
     * @brief Returns session handle for session in plugin.
     * @return Session handle
     */
    Session * session() const;

signals:

    void peerPluginAdded(PeerPlugin *);

    void peerPluginRemoved(const libtorrent::tcp::endpoint &);

private:

    friend class Torrent;

    void addPeerPlugin(const extension::status::PeerPlugin &);

    void removePeerPlugin(const libtorrent::tcp::endpoint &);

    void removePeerPlugin(std::map<libtorrent::tcp::endpoint, std::unique_ptr<PeerPlugin>>::iterator it);

    void update(const extension::status::TorrentPlugin &);

    /// Member fields

    libtorrent::sha1_hash _infoHash;

    std::map<libtorrent::tcp::endpoint, std::unique_ptr<PeerPlugin>> _peers;

    std::unique_ptr<Session> _session;

    boost::shared_ptr<extension::Plugin> _plugin;
};

}
}

#endif // JOYSTREAM_CORE_TORRENTPLUGIN_HPP
