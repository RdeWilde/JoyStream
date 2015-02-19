#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "PaymentChannel/Payor.hpp"

//class BuyerPeerPlugin;
#include <QTime>

/**
 * @brief Torrent plugin for buyer mode.
 */
class BuyerTorrentPlugin : public TorrentPlugin
{
public:

    /**
     * @brief
     */
    enum class State {

        // Inviting all peers with good enough peers,
        building_contract,

        // Requesting and downloading pieces
        downloading_pieces,

        // Waiting to claim one or more refunds
        // since contract outputs have not been spent
        waiting_for_refunds_to_be_spendable,

        // All outputs have been spent
        done
    };

    /**
     * @brief Configuration of buyer torrent plugin.
     */
    class Configuration : public TorrentPlugin::Configuration {

    public:

        // Constructor from copy
        Configuration(const Configuration & c);

        // Constructor from members
        Configuration(State stage, quint64 maxPrice, quint32 maxLock, quint64 maxFeePerByte, qint32 numSellers);

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
        State state() const;
        void setState(const State & state);

        quint64 maxPrice() const;
        void setMaxPrice(quint64 maxPrice);

        quint32 maxLock() const;
        void setMaxLock(quint32 maxLock);

        quint64 maxFeePerByte() const;
        void setMaxFeePerByte(quint64 maxFeePerByte);

        quint32 numSellers() const;
        void setNumSellers(quint32 numSellers);

    private:

        // What stage is plugin
        State _state;

        // Maximum price accepted (satoshies)
        quint64 _maxPrice;

        // Maximum lock time (the number of seconds elapsed since 1970-01-01T00:00 UTC)
        quint32 _maxLock;

        // Maximum fee per byte in contract transaction (satoshies)
        quint64 _maxFeePerByte;

        // Number of seller in payment channel
        quint32 _numSellers;
    };

    // Constructor
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::weak_ptr<libtorrent::torrent> & torrent,
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

    // Getters and setters
    virtual PluginMode pluginMode() const;

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
    //Configuration _configuration;
};

#endif // BUYER_TORRENT_PLUGIN_HPP
