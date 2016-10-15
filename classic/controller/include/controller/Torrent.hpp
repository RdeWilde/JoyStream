/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP

#include <libtorrent/socket.hpp>
#include <libtorrent/torrent_status.hpp>
#include <QObject>
#include <boost/optional.hpp>
#include <cstdint>
#include <memory>

class BitcoinDisplaySettings;

namespace joystream {
namespace core {
    class Torrent;
    class Peer;
    class TorrentPlugin;
    class PeerPlugin;
    class Session;
    class Connection;
}
namespace protocol_session {
    enum class SessionMode;
}
namespace classic {
namespace gui {
    class PeerTableModel;
    class PeerTableRowModel;
    class BuyerTableModel;
    class BuyerTableRowModel;
    class ObserverTableModel;
    class ObserverTableRowModel;
    class SellerTableModel;
    class SellerTableRowModel;
    class ConnectionTableRowModel;
    class ConnectionTableModel;
    class TorrentTableRowModel;
    class TorrentTableModel;
}
namespace controller {

class ApplicationController;
class Peer;

class Torrent : public QObject {

    Q_OBJECT

public:

    Torrent(core::Torrent * torrent,
            gui::TorrentTableModel * torrentTableModel,
            const BitcoinDisplaySettings * settings);

    ~Torrent();

    /**
     * @brief Update display status of this torrent in the main window
     * @param show whether to show (true) or hide (false) in main window
     * @throws std::runtime_error if show && visibleInMainWindow()
     * @throws std::runtime_error if !show && !visibleInMainWindow()
     */
    void showInMainWindow(bool show);

    /**
     * @brief Returns whether this torrent is visible in main window
     * @return whether visible in main window
     */
    bool visibleInMainWindow() const noexcept;

public slots:

    /// core::Torrent events

    void setName(const std::string & name);

    void setSize(boost::int64_t totalSize);

    void setState(libtorrent::torrent_status::state_t state, float progress);

    void setPaused(bool);

    void setDownloadSpeed(int speed);

    void setUploadSpeed(int speed);

    void addPeer(core::Peer * peer);

    void removePeer(const libtorrent::tcp::endpoint & endPoint);

    void setTorrentPluginPresent(core::TorrentPlugin * plugin);

    void setTorrentPluginAbsent();

    /// core::TorrentPlugin events

    void addPeerPlugin(core::PeerPlugin * peerPlugin);

    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    /// core::Session events

    void setSessionMode(protocol_session::SessionMode mode);

    void addConnection(core::Connection * connection);

    void removeConnection(const libtorrent::tcp::endpoint & endPoint);

    /// core::Session|core::Connection events

    quint32 numberOfBuyers() const;
    void setNumberOfBuyers(quint32 numberOfBuyers);

    quint32 numberOfSellers() const;
    void setNumberOfSellers(quint32 numberOfSellers);

    quint32 numberOfObservers() const;
    void setNumberOfObservers(quint32 numberOfObservers);

    quint32 numberOfUnannounced() const;
    void setNumberOfUnannounced(quint32 numberOfUnannounced);

    /// core::CBStateMachine events/values

    quint64 balance() const;
    void setBalance(quint64 balance);

private:

    core::Torrent * _torrent;

    /// Derived state: not immediately avaiable from references

    // Number of peers in buyer mode (from corresponding core::CBStateMachine)
    quint32 _numberOfBuyers,
            _numberOfSellers,
            _numberOfObservers,
            _numberOfUnannounced;

    // Total balance of (unconfirmed) spending or income, for
    // buying and selling mode respectively,
    // (not including transaction fees: fix later)
    quint64 _balance;

    /// View models

    // Torrent row view model: _torrentRowModel is set <=> visible in main window
    gui::TorrentTableModel * _torrentTableModel;
    std::unique_ptr<gui::TorrentTableRowModel> _torrentRowModel;

    // Table view models
    gui::PeerTableModel _classicPeerTableModel;
    gui::BuyerTableModel _buyerTableModel;
    gui::ObserverTableModel _observerTableModel;
    gui::SellerTableModel _sellerTableModel;
    gui::ConnectionTableModel _sellersTableModel, _buyersTableModel;

    std::map<libtorrent::tcp::endpoint, std::unique_ptr<Peer>> _peers;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_TORRENT_HPP

