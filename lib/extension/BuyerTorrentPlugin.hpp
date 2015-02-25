#ifndef BUYER_TORRENT_PLUGIN_HPP
#define BUYER_TORRENT_PLUGIN_HPP

#include "TorrentPlugin.hpp"
#include "PaymentChannel/Payor.hpp"
#include "BuyerPeerPlugin.hpp"

//#include <QTime>

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
     * @brief Plugin status, that is a snapshot
     * of important information.
     */
    class Status {

    public:

        Status(State state,
                const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers,
                const Payor::Status & payor);

        // Getters and Setters
        State state() const;
        void setState(State state);

        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> peers() const;
        void setPeers(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> & peers);

        Payor::Status payor() const;
        void setPayor(const Payor::Status & payor);

    private:

        // State of plugin
        State _state;

        // Status of peers
        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Status> _peers;

        // Status of the payor
        Payor::Status _payor;
    };

    /**
     * @brief Configuration of buyer torrent plugin.
     */
    class Configuration : public TorrentPlugin::Configuration {

    public:

        // Constructor from copy
        Configuration(const Configuration & c);

        // Constructor from members
        Configuration(bool enableBanningSets,
                      State state,
                      const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> & peerConfigurations,
                      const Payor::Configuration payorConfiguration);

        // Constructor for a fresh plugin.
        Configuration(quint32 numberOfSellers,
                      const OutPoint & fundingOutput,
                      const KeyPair & fundingOutputKeyPair,
                      quint64 maxPrice,
                      quint32 maxLock);

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

        State state() const;
        void setState(const State & state);

        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> peerConfigurations() const;
        void setPeerConfigurations(const QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> &peerConfigurations);

        Payor::Configuration payorConfiguration() const;
        void setPayorConfiguration(const Payor::Configuration &payorConfiguration);

    private:

        // What stage is plugin
        State _state;

        // Configuration of peers
        QMap<libtorrent::tcp::endpoint, BuyerPeerPlugin::Configuration> _peerConfigurations;

        // Configuration of payor
        Payor::Configuration _payorConfiguration;
    };

    // Constructor from members
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                       const Configuration & configuration,
                       QLoggingCategory & category);

    /**
    BuyerTorrentPlugin(Plugin * plugin,
                       const boost::weak_ptr<libtorrent::torrent> & torrent,
                       quint32 _numSellers,
                       QLoggingCategory & category);
    */

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
    //virtual boost::weak_ptr<libtorrent::peer_plugin> peerPlugin(const libtorrent::tcp::endpoint & endPoint) const;

    // Generate plugin status
    Status status() const;

    // Getters and setters
    virtual PluginMode pluginMode() const;

    State state() const;
    void setState(const State & state);

    // Allows
    const Payor & payor() const;


private:

    // What stage is plugin
    State _state;

    // Maps endpoint to weak peer plugin pointer, is peer_plugin, since this is
    // the type of weak_ptr libtrrrent requires, hence might as well put it
    // in this type, rather than corresponding subclass of TorrentPlugin.
    QMap<libtorrent::tcp::endpoint, boost::weak_ptr<BuyerPeerPlugin> > _peers;

    // Payment channel
    Payor _payor;


    // Time since plugin was created, is used to keep track of when to start picking sellers.
    QTime _timeSincePluginStarted;

    // what requests have been sent out for pieces we still
    // dont have, and how long have we been waiting (so that we can discard slow bastards).

    // What refunds have been spent,and what have not.
    // Use timer to keep checking back?
};

#endif // BUYER_TORRENT_PLUGIN_HPP
