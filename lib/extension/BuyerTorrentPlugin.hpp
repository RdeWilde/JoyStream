/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "PaymentChannel/Payor.hpp"
#include "BuyerPeerPlugin.hpp"

//#include <QTime>

#include <queue>          // std::priority_queue

class Wallet;
class UnspentP2PKHOutput;

#include <boost/shared_array.hpp>

/**
* HOW DO YOU FORWARD DECLARE A TEMPLATED TYPE?
namespace boost {
    class shared_array<char>;
}
*/

/**
 * @brief Torrent plugin for buyer mode.
 */
class BuyerTorrentPlugin : public TorrentPlugin
{
public:

    /**
     * @brief Plugin state
     */
    enum class State {

        // _payor has reached, or passed, Payor:State::paying status
        waiting_for_payor_to_be_ready,

        // Requesting and downloading pieces
        downloading_pieces,

        // Have full torrent
        done
    };

    /**
     * @brief Represents piece in torrent
     */
    class Piece {

    public:

        enum class State {

            // We do not have piece, and it has given assignment status
            unassigned,
            assigned,

            // We do have piece
            fully_downloaded_and_valid,
        };

        // Default constructor
        Piece();

        // Constructors based on members
        //Piece(int index, int length, int numberOfBlocks, State state, BuyerPeerPlugin * peerPlugin);

        // Constructors based on members
        Piece(int index, State state, BuyerPeerPlugin * peerPlugin);

        // Getters and setters
        int index() const;
        void setIndex(int index);

        /**
        int length() const;
        void setLength(int length);

        int numberOfBlocks() const;
        void setNumberOfBlocks(int numberOfBlocks);
        */

        State state() const;
        void setState(State state);

        BuyerPeerPlugin * peerPlugin() const;
        void setPeerPlugin(BuyerPeerPlugin * peerPlugin);

    private:

        // Index of piece
        int _index;

        // Byte length of piece (should be the same for all but last piece)
        int _length;

        // Number of blocks in piece
        //int _numberOfBlocks;

        // Piece state
        State _state;

        // Peer plugin assigned to this piece
        BuyerPeerPlugin * _peerPlugin;
    };

    /**
     * @brief Plugin status, that is a snapshot
     * of important information.
     */
    class Status { //  : public TorrentPlugin::Status

    public:

        Status();

        Status(State state,
               const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peerPluginStatuses,
               const Payor::Status & payor);

        // Getters and setters
        State state() const;
        void setState(State state);

        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peerPluginStatuses() const;
        void setPeerPluginStatuses(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peerPluginStatuses);

        Payor::Status payor() const;
        void setPayor(const Payor::Status & payor);

    private:

        // State of plugin
        State _state;

        // Status of peers
        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> _peerPluginStatuses;

        // Status of the payor
        Payor::Status _payor;
    };

    /**
     * @brief Configuration of buyer torrent plugin.
     */
    class Configuration : public TorrentPlugin::Configuration {

    public:

        // Default constructor
        Configuration();

        // Constructor from members
        Configuration(bool enableBanningSets,
                      quint64 maxPrice,
                      quint32 maxLock,
                      quint64 maxFeePerKb,
                      quint32 numberOfSellers);

        // Constructor from copy <=== Why is this here again? who is using this
        //Configuration(const Configuration & c);

        // Constructor from dictionary
        Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        /**
         * Write configuration into dictionary
         * ===============================================================
         *
         * Buyer torrent plugin configuration as it persists across sessions on disk
         * encoded as entry::dictionary_type with the following keys:
         *
         * IMPLEMENT LATER
         *
         */
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

        // Getters and setters
        virtual PluginMode pluginMode() const;

        quint64 maxPrice() const;
        void setMaxPrice(quint64 maxPrice);

        quint32 maxLock() const;
        void setMaxLock(quint32 maxLock);

        quint64 maxFeePerKb() const;
        void setMaxFeePerKb(quint64 maxFeePerKb);

        quint32 numberOfSellers() const;
        void setNumberOfSellers(quint32 numberOfSellers);

    private:


        // Maximum price accepted (satoshies)
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        quint32 _maxLock;

        // Maximum fee per kB in contract transaction (satoshies)
        quint64 _maxFeePerKb;

        // Number of sellers
        quint32 _numberOfSellers;
    };

    // Constructor from members
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::shared_ptr<libtorrent::torrent> & torrent,
                       Wallet * wallet,
                       const Configuration & configuration,
                       const UnspentP2PKHOutput & utxo,
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

    // Get peer_plugin if present, otherwise NULL pointer is wrapped
    //virtual boost::sharedPluginPtr<libtorrent::peer_plugin> peerPlugin(const libtorrent::tcp::endpoint & endPoint) const;

    // Checks if seller can be invited on given terms
    //bool inviteSeller(quint32 minPrice, quint32 minLock) const;

    // Attempts to add seller to contract
    bool sellerWantsToJoinContract(BuyerPeerPlugin * peer, quint64 price, quint32 refundLockTime, const PublicKey & contractPk, const PublicKey & finalPk);

    // Verifies signature, and also broadcasts contract if full set of signatures has been aquired
    bool sellerProvidedRefundSignature(BuyerPeerPlugin * peer, const Signature & refundSignature);

    // Find peers which presently have not been assigned a piece, and assign piece.
    bool assignPieceToPeerPlugin(BuyerPeerPlugin * peer);

    // Starting after startIndex, and finds the first piece which has not been downloaded and not assigned.
    // If no piece is found greater than startIndex, it loops around from piece 0
    // to see if there are any hits there. If this fails, it throws and exception
    // to signify that all pieces have been assigned
    int getNextUnassignedPiece(int startIndex) const;

    // Marks corresponding piece as downloaded
    void pieceDownloaded(int index);

    // Increments payment counter in payment channel, and generates signature
    Signature makePaymentAndGetPaymentSignature(BuyerPeerPlugin * peerPlugin);

    // Ask libtorrent to validate piece
    //bool checkLengthAndValidatePiece(int pieceIndex, const boost::shared_array<char> & piece, int length);
    void fullPieceArrived(BuyerPeerPlugin * peer, const boost::shared_array<char> & piece, int length);

    // Amount of funds (satoshies) sent since start
    quint64 totalSentSinceStart() const;

    // Amount of funds (satoshies) sent since start over given channel
    quint64 channelBalance(int i) const;

    // Amount of funds (satoshies) presently locked
    // in channels started during this session.
    // Obviosuly does not include change in channels!
    quint64 totalCurrentlyLockedInChannels() const;

    // Removes entry for plugin with give endpoint from _peers QMap
    //bool removePluginIfInPeersMap(const libtorrent::tcp::endpoint & endPoint);

    /**
     * Peer plugin Libtorrent event handlers
     */

    // peer_plugin::on_disconnect
    void on_peer_plugin_disconnect(BuyerPeerPlugin * peerPlugin, libtorrent::error_code const & ec);

    // Generate plugin status
    Status status() const;

    // Getters and setters
    virtual PluginMode pluginMode() const;
    QList<libtorrent::tcp::endpoint> endPoints() const;
    //const PeerPlugin * peerPlugin(const libtorrent::tcp::endpoint & endPoint) const;

    State state() const;
    void setState(const State & state);

    quint64 maxPrice() const;
    void setMaxPrice(quint64 maxPrice);

    quint32 maxLock() const;
    void setMaxLock(quint32 maxLock);

    quint64 maxFeePerKb() const;
    void setMaxFeePerKb(quint64 maxFeePerKb);

    quint32 numberOfSellers() const;
    void setnumberOfSellers(quint32 numberOfSellers);

    /**
    int blockSize() const;
    void setBlockSize(int blockSize);
    */

    int assignmentLowerBound() const;
    void setAssignmentLowerBound(int assignmentLowerBound);

private:

    // What stage is plugin
    State _state;

    // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
    // the type of weak_ptr libtrrrent requires, hence might as well put it
    // in this type, rather than corresponding subclass of TorrentPlugin.
    QMap<libtorrent::tcp::endpoint, boost::weak_ptr<BuyerPeerPlugin> > _peers;

    // List of peer plugins scheduled for deletion
    // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
    QList<boost::weak_ptr<BuyerPeerPlugin> > _peersScheduledForDeletion;

    // Wallet
    Wallet * _wallet;



        /**
         * Remove these later? perhaps just keep in Payor.
         */

        // Maximum price accepted (satoshies)
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        quint32 _maxLock;

        // Maximum fee per byte in contract transaction (satoshies)
        quint64 _maxFeePerKb;

        // Number of sellers
        quint32 _numberOfSellers;


    // Payment channel
    Payor _payor;

    // Maps given position in payor to given end point
    /**
     * LATER, ADD SOME SORT OF ELEMENT FIELD TO THE CHANNELS OF THE PAYOR,
     * SO THAT THERE IS TIGHTER ASSOCIATION BETWEEN CHANNELS AND
     * Lets just use sloppy pointers for now!!!!!!!!
     */
    QVector<BuyerPeerPlugin *> _slotToPluginMapping;

    // Time since plugin was created, is used to keep track of when to start picking sellers.
    QTime _timeSincePluginStarted;

    /**
     * Piece management
     */

    // Pieces in torrent file
    QVector<Piece> _pieces;

    // The blocksize in the torrent
    //int _blockSize;

    // The number of pieces which have not been downloaded and not been assigned to a peer
    quint32 _numberOfUnassignedPieces;

    // Set of peer plugins which have not been assigned a piece.
    // the tick() callback routinely attempts to assign a piece
    // to a peer plugin in this set.
    QSet<BuyerPeerPlugin *> _peerPluginsWithoutPieceAssignment;

    // Keeps track of lower bound for piece indexes which may be assigned.
    // Is updated when full pieces are downloaded contigously, and
    // is used with getNextUnassignedPiece() to find next piece to assign.
    //
    // Is required to ensure in order downloading from correct position in file, e.g.,
    // if user has seeked to this position recently.
    int _assignmentLowerBound;

    // Delete and disconnect peers which have PeerPlugin::_scheduledForDeletingInNextTorrentPluginTick == true
    // ** NEEDS TO BE ABSTRACTED TO PARENT CLASS **
    int deleteAndDisconnectPeers();
};

#include <QMetaType>
Q_DECLARE_METATYPE(BuyerTorrentPlugin::State)

#endif // BUYER_TORRENT_PLUGIN_HPP
