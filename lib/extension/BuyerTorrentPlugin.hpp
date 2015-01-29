#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "PaymentChannel/PaymentChannelPayor.hpp"
//#include "BitCoin/PublicKey.hpp"

class TorrentPluginConfiguration;
class BuyerPeerPlugin;

/**
 * @brief Torrent plugin for buyer mode.
 */
class BuyerTorrentPlugin : public TorrentPlugin
{
public:

    /**
     * @brief Configuration of buyer torrent plugin.
     */
    class Configuration {

    public:

        /**
         * @brief The Stage enum
         */
        enum class Stage {

            // Inviting all peers with good enough peers,
            //
            building_contract,

            // Requesting and downloading pieces
            downloading_pieces,

            // Waiting to claim one or more refunds
            // since contract outputs have not been spent
            waiting_for_refunds_to_be_spendable,

            // All outputs have been spent
            done,
        };

        // Constructor from copy
        Configuration(const Configuration & c);

        // Constructor from members
        Configuration(quint64 maxPrice, QTime maxLock, quint64 maxFeePerByte, qint32 numSellers);

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
        quint64 maxPrice() const;
        void setMaxPrice(const quint64 &maxPrice);

        QTime maxLock() const;
        void setMaxLock(const QTime &maxLock);

        quint64 maxFeePerByte() const;
        void setMaxFeePerByte(const quint64 &maxFeePerByte);

        qint32 numSellers() const;
        void setNumSellers(const qint32 &numSellers);

    private:

        // What stage is plugin
        Stage _stage;

        // Maximum price accepted (satoshies)
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        QTime _maxLock;

        // Maximum fee per byte in contract transaction (satoshies)
        quint64 _maxFeePerByte;

        // Number of seller in payment channel
        qint32 _numSellers;
    };

    // Constructor
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                       const TorrentPluginConfiguration & torrentPluginConfiguration,
                       const Configuration & configuration,
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

    // Removes peer plugin by
    void removePeerPlugin(const libtorrent::tcp::endpoint & endPoint);

    /**
     * Mutually exclusive set of states for torrent plugin,
     * in terms of cause of tick processing not advancing.
     */
    enum class State {

        // Sending out join_contract messages while trying to get enough
        // peers to sign contract refunds to start the channel
        populating_payment_channel,

        // Requesting, validating, downlading, saving and paying for pieces
        downloading_pieces,

        // While contract outputs have not all been spent, we still wait to use refunds
        waiting_to_spend_refunds,

        // Nothing more to do
        finished
    };

    // Getters and setters
    State state() const;

    Configuration configuration() const;
    void setConfiguration(const Configuration &configuration);

private:

    // Buyer plugin state
    State _state;

    // Payment channel
    Payor _channel;

    // Time since plugin was created, is used to keep track of when to start picking sellers.
    QTime _timeSincePluginStarted;

    // what requests have been sent out for pieces we still
    // dont have, and how long have we been waiting (so that we can discard slow bastards).

    // What refunds have been spent,and what have not.
    // Use timer to keep checking back?

    // Configuration
    Configuration _buyerTorrentPluginConfiguration;
};

#endif // BUYER_TORRENT_PLUGIN_HPP
