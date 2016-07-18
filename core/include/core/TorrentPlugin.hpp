/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_TORRENTPLUGIN_HPP
#define JOYSTREAM_CORE_TORRENTPLUGIN_HPP

#include <extension/extension.hpp>
#include <core/PeerPlugin.hpp>
#include <core/Session.hpp>

#include <QObject>

namespace joystream {
namespace core {

class TorrentPlugin : public QObject {

    Q_OBJECT

private:

    TorrentPlugin(const extension::status::TorrentPlugin & status,
                  const boost::shared_ptr<extension::Plugin> & plugin);

public:

    void start(const extension::request::SubroutineHandler &);

    void stop(const extension::request::SubroutineHandler &);

    void pause(const extension::request::SubroutineHandler &);

    void updateTerms(const protocol_wire::BuyerTerms & terms,
                     const extension::request::SubroutineHandler &);

    void updateTerms(const protocol_wire::SellerTerms & terms,
                     const extension::request::SubroutineHandler &);

    void toObserveMode(const extension::request::SubroutineHandler &);

    void toSellMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                    const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                    const protocol_session::SellingPolicy & sellingPolicy,
                    const protocol_wire::SellerTerms & terms,
                    const extension::request::SubroutineHandler &);

    void toBuyMode(const protocol_session::GenerateKeyPairsCallbackHandler & generateKeyPairsCallbackHandler,
                   const protocol_session::GenerateP2PKHAddressesCallbackHandler & generateP2PKHAddressesCallbackHandler,
                   const Coin::UnspentP2PKHOutput & funding,
                   const protocol_session::BuyingPolicy & policy,
                   const protocol_wire::BuyerTerms & terms,
                   const extension::request::SubroutineHandler &);

signals:

    void peerPluginAdded(const std::shared_ptr<PeerPlugin> &);

    void peerPluginRemoved(const libtorrent::tcp::endpoint &);

private:

    friend class Torrent;

    void addPeerPlugin(const extension::status::PeerPlugin &);

    void removePeerPlugin(const libtorrent::tcp::endpoint &);

    void update(const extension::status::TorrentPlugin &);

    /// Member fields

    libtorrent::sha1_hash _infoHash;

    std::map<libtorrent::tcp::endpoint, std::shared_ptr<PeerPlugin> > _peers;

    std::shared_ptr<Session> _session;

    boost::shared_ptr<extension::Plugin> _plugin;
};

}
}

#endif // JOYSTREAM_CORE_TORRENTPLUGIN_HPP
