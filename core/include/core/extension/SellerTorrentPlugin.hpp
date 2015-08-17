/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef SELLER_TORRENT_PLUGIN_HPP
#define SELLER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "SellerPeerPlugin.hpp"

namespace Wallet {
class Manager;
}

namespace libtorrent {
    struct read_piece_alert;
}

class SellerTorrentPlugin : public TorrentPlugin
{
public:

    class Status {

    public:

        // Default constructor
        Status();

        // Constructor from members
        Status(quint64 minPrice,
               quint32 minLock,
               quint64 minFeePerByte,
               quint32 maxNumberOfSellers,
               quint32 maxContractConfirmationDelay,
               qint64 balance,
               const QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> & peerPluginStatuses);

        // Getters and setters
        quint64 minPrice() const;
        void setMinPrice(quint64 minPrice);

        quint32 minLock() const;
        void setMinLock(quint32 minLock);

        quint64 minFeePerByte() const;
        void setMinFeePerByte(quint64 minFeePerByte);

        quint32 maxNumberOfSellers() const;
        void setMaxNumberOfSellers(quint32 maxNumberOfSellers);

        quint32 maxContractConfirmationDelay() const;
        void setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay);

        qint64 balance() const;
        void setBalance(qint64 balance);

        QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> peerPluginStatuses() const;
        void setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> & peerPluginStatuses);

    private:

        // Maximum price accepted (satoshies)
        quint64 _minPrice;

        // Minimum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        quint32 _minLock;

        // Minimum fee per byte in contract transaction (satoshies)
        quint64 _minFeePerByte;

        // Number of sellers
        quint32 _maxNumberOfSellers;

        // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
        quint32 _maxContractConfirmationDelay;

        // Net revenue across all peers since session started
        qint64 _balance;

        // Status of peer plugins
        QMap<libtorrent::tcp::endpoint, SellerPeerPlugin::Status> _peerPluginStatuses;
    };

    /**
     * @brief Configuration of seller torrent plugin.
     */
    class Configuration : public TorrentPlugin::Configuration {

    public:

        /**
         * Mutually exclusive set of states for seller torrent plugin,
         * in terms of cause of tick processing not advancing.

        enum class State {

            something about whether we accept new peers
            to sell to at the moment or something?

        };

        */

        // Default constructor
        Configuration();

        // Constructor from members
        Configuration(bool enableBanningSets,
                      quint64 minPrice,
                      quint32 minLock,
                      quint64 minFeePerByte,
                      quint32 maxNumberOfSellers,
                      quint32 maxContractConfirmationDelay);

        // Constructor from dictionary
        Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Getters and setters
        virtual PluginMode pluginMode() const;

        quint64 minPrice() const;
        void setMinPrice(quint64 minPrice);

        quint32 minLock() const;
        void setMinLock(quint32 minLock);

        quint64 minFeePerByte() const;
        void setMinFeePerByte(quint64 minFeePerByte);

        quint32 maxNumberOfSellers() const;
        void setMaxNumberOfSellers(quint32 maxNumberOfSellers);

        quint32 maxContractConfirmationDelay() const;
        void setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay);

    private:

        // Maximum price accepted (satoshies)
        quint64 _minPrice;

        // Minimum lock time
        // Whould have been nice to use QTime, however it is limited to 24h cycle.
        quint32 _minLock;

        // Minimum fee per byte in contract transaction (satoshies)
        quint64 _minFeePerByte;

        // Number of sellers
        quint32 _maxNumberOfSellers;

        // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
        quint32 _maxContractConfirmationDelay;
    };

    // Constructor
    SellerTorrentPlugin(Plugin * plugin,
                        const boost::shared_ptr<libtorrent::torrent> & torrent,
                        Wallet::Manager * wallet,
                        const SellerTorrentPlugin::Configuration & configuration,
                        QLoggingCategory & category);

    /**
     * All virtual functions below should ONLY be called by libtorrent network thread,
     * never by other threads, as this causes synchronization failures.
     */
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection);
    virtual void on_piece_pass(int index);
    virtual void on_piece_failed(int index);
    virtual void tick();
    virtual bool on_resume();
    virtual bool on_pause();
    virtual void on_files_checked();
    virtual void on_state(int s);
    virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags);

    SellerPeerPlugin * createRegularSellerPeerPlugin(libtorrent::bt_peer_connection * connection);
    SellerPeerPlugin * createSellerPeerPluginScheduledForDeletion(libtorrent::bt_peer_connection * connection);

    // Get peer_plugin if present, otherwise NULL pointer is wrapped
    //virtual boost::shared_ptr<libtorrent::peer_plugin> peerPlugin(const libtorrent::tcp::endpoint & endPoint) const;

    // Schedules asynchronous reads for all the blocks in the piece
    // presently assigned to the given peer
    //int disk_async_read_piece(SellerPeerPlugin * peer);

    /**
    // Issues asynchronous read operation to piece manager
    void async_read(const libtorrent::peer_request & r,
                    const boost::function<void(int, libtorrent::disk_io_job const&)> & handler,
                    int cache_line_size = 0,
                    int cache_expiry = 0);
    */

    // Schedules asynchronous read of piece to this peer
    void readPiece(SellerPeerPlugin * peer, int piece);

    // Call back after piece read
    void pieceRead(const libtorrent::read_piece_alert * alert);

    // Amount of funds (satoshies) received since start
    //quint64 totalReceivedSinceStart() const;

    /**
     * Peer plugin Libtorrent event handlers
     */

    // peer_plugin::on_disconnect
    void on_peer_plugin_disconnect(SellerPeerPlugin * peerPlugin, libtorrent::error_code const & ec);

    // Updates balance of torrent plugin
    qint64 addToBalance(quint64 revenue);

    // Creates status for plugin
    Status status() const;

    // Creates configuratin for plugin
    Configuration configuration() const;
    QList<libtorrent::tcp::endpoint> endPoints() const;
    //const PeerPlugin * peerPlugin(const libtorrent::tcp::endpoint & endPoint) const;

    // Getters and setters
    virtual PluginMode pluginMode() const;

    quint64 minPrice() const;
    void setMinPrice(quint64 minPrice);

    quint32 minLock() const;
    void setMinLock(quint32 minLock);

    quint64 minFeePerByte() const;
    void setMinFeePerByte(quint64 minFeePerByte);

    quint32 maxNumberOfSellers() const;
    void setMaxNumberOfSellers(quint32 maxNumberOfSellers);

    quint32 maxContractConfirmationDelay() const;
    void setMaxContractConfirmationDelay(quint32 maxContractConfirmationDelay);

private:

    // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
    // the type of weak_ptr libtrrrent requires, hence might as well put it
    // in this type, rather than corresponding subclass of TorrentPlugin.
    QMap<libtorrent::tcp::endpoint, boost::weak_ptr<SellerPeerPlugin> > _peers;

    // List of peer plugins scheduled for deletion
    // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
    QList<boost::weak_ptr<SellerPeerPlugin> > _peersScheduledForDeletion;

    // Maintains mapping between piece index and peers that are waiting for this.
    // Will typically just be one, but may be multiple - hence QSet is used
    QMap<int, QSet<SellerPeerPlugin *> > _outstandingPieceRequests;

    // Wallet
    Wallet::Manager * _wallet;

    // Maximum price accepted (satoshies)
    quint64 _minPrice;

    // Minimum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
    quint32 _minLock;

    // Minimum fee per byte in contract transaction (satoshies)
    quint64 _minFeePerByte;

    // Number of sellers
    quint32 _maxNumberOfSellers;

    // Maximum time (s) for which seller is willing to seed without contract getting at least one confirmation
    quint32 _maxContractConfirmationDelay;

    /**
     * Statistics
     */

    // Net revenue across all peers since session started
    qint64 _balance;

    // Delete and disconnect peers which have PeerPlugin::_scheduledForDeletingInNextTorrentPluginTick == true
    // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
    int deleteAndDisconnectPeers();

};

//#include <QMetaType>
//Q_DECLARE_METATYPE(const SellerTorrentPlugin::Configuration &)

#endif // SELLER_TORRENT_PLUGIN_HPP
