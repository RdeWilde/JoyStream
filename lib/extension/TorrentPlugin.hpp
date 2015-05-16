#ifndef TORRENT_PLUGIN_HPP
#define TORRENT_PLUGIN_HPP

#include "BEPSupportStatus.hpp"
//#include "TorrentPluginConfiguration.hpp"
#include "Utilities.hpp" // uint qHash(const libtorrent::tcp::endpoint & endpoint)

#include <libtorrent/extensions.hpp>
#include <libtorrent/torrent.hpp>

//#include <boost/weak_ptr.hpp>

#include <QObject>
#include <QMap>
#include <QSet>

// Forward declaration
class Plugin;
class PeerPlugin;
//class PeerPluginId;
class TorrentPluginStatus;
class TorrentPluginRequest;
//class PeerPluginConfiguration;
//class SetConfigurationTorrentPluginRequest;
//class SetPluginModeTorrentPluginRequest;
class TorrentPluginStatusAlert;
class TorrentPluginAlert;
enum class PluginMode;

/**
 * @brief Abstract type for all torrent plugin types (buyer, seller, observer).
 */
class TorrentPlugin : public QObject, public libtorrent::torrent_plugin {

    Q_OBJECT

public:

    class Status {

    public:

        // Default constructor
        Status();

        // Constructor from members
        Status(quint32 numberOfClassicPeers,
               quint32 numberOfObserverPeers,
               quint32 numberOfSellerPeers,
               quint32 numberOfBuyerPeers);

        // Getters and setters
        quint32 numberOfClassicPeers() const;
        void setNumberOfClassicPeers(quint32 numberOfClassicPeers);

        quint32 numberOfObserverPeers() const;
        void setNumberOfObserverPeers(quint32 numberOfObserverPeers);

        quint32 numberOfSellerPeers() const;
        void setNumberOfSellerPeers(quint32 numberOfSellerPeers);

        quint32 numberOfBuyerPeers() const;
        void setNumberOfBuyerPeers(quint32 numberOfBuyerPeers);

    private:

        // #peers connected to in classic - i.e. non joystream extension enabled mode
        quint32 _numberOfClassicPeers;

        // #peers connected to with plugin and in observer mode
        quint32 _numberOfObserverPeers;

        // #peers connected to with plugin and in seller mode
        quint32 _numberOfSellerPeers;

        // #peers connected to with plugin and in buyer mode
        quint32 _numberOfBuyerPeers;
    };

    /**
     * @brief
     */
    class Configuration {

    public:

        // Default constructor
        Configuration();

        // Copy constructor
        Configuration(const Configuration & configuration);

        // Constructor from members
        Configuration(bool enableBanningSets);

        // Constructor from dictionary
        Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Assignment
        // KeyPair & KeyPair::operator=(const KeyPair& rhs)
        //Configuration(const Configuration & rhs);

        // Determnes the plugin mode of TorrentPlugin::Configuration object encoded in dictionary
        static PluginMode pluginMode(libtorrent::entry::dictionary_type & dictionaryEntry);

        // Turn into dictionary
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

        // Getters and setters
        virtual PluginMode pluginMode() const = 0;

        bool enableBanningSets() const;
        void setEnableBanningSets(bool enableBanningSets);

    protected:

        // Whether peers are banned for bad conduct
        bool _enableBanningSets;
    };

    // Constructor from member fields
    TorrentPlugin(Plugin * plugin,
                  const boost::shared_ptr<libtorrent::torrent> & torrent,
                  const TorrentPlugin::Configuration & configuration,
                  QLoggingCategory & category);

    /**
     * Virtual routines
     */

    // Destructor
    virtual ~TorrentPlugin();

    // Libtorrent callbacks
    virtual boost::shared_ptr<libtorrent::peer_plugin> new_connection(libtorrent::peer_connection * connection) = 0;
    virtual void on_piece_pass(int index) = 0;
    virtual void on_piece_failed(int index) = 0;
    virtual void tick() = 0;
    virtual bool on_resume() = 0;
    virtual bool on_pause() = 0;
    virtual void on_files_checked() = 0;
    virtual void on_state(int s) = 0;
    virtual void on_add_peer(const libtorrent::tcp::endpoint & endPoint, int src, int flags) = 0;

    // Removes peer plugin
    //virtual void removePeerPlugin(PeerPlugin * plugin) = 0;

    /**
     * Routines called by libtorrent network thread from other plugin objects
     */
    // Get peer plugin, throws std::exception if there is no match
    //virtual boost::shared_ptr<libtorrent::peer_plugin> peerPlugin(const libtorrent::tcp::endpoint & endPoint) const = 0;

    // Process torrent plugin requests
    // void processTorrentPluginRequest(const TorrentPluginRequest * request);

    // Adds peer to respective set, and returns whether it was actually added or existed in the set from before.
    void addToPeersWithoutExtensionSet(const libtorrent::tcp::endpoint & endPoint);
    void addToIrregularPeersSet(const libtorrent::tcp::endpoint & endPoint);

    // Getters and setters
    Plugin * plugin();

    virtual PluginMode pluginMode() const = 0;
    //virtual const TorrentPlugin::Configuration getTorrentPluginConfiguration() = 0;

    boost::shared_ptr<libtorrent::torrent> torrent() const;
    void setTorrent(const boost::shared_ptr<libtorrent::torrent> & torrent);

    bool enableBanningSets() const;
    void setEnableBanningSets(bool enableBanningSets);

protected:

    // Parent plugin for BitSwapr
    // Should this be boost::shared_ptr, since life time of object is managed by it?
    // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
    Plugin * _plugin;

    // Torrent for this torrent_plugin
    boost::shared_ptr<libtorrent::torrent> _torrent;

    // Set of all endpoints known to not have extension. Is populated by previous failed extended handshakes.
    QSet<libtorrent::tcp::endpoint> _peersWithoutExtension;

    // Set of endpoints banned for irregular conduct during extended protocol
    QSet<libtorrent::tcp::endpoint> _irregularPeer;

    // Logging category
    QLoggingCategory & _category;

    // Torrent info hash
    //libtorrent::sha1_hash _infoHash;

    /**
    // Plugin is active and therefore does tick() processing.
    // Is set by controller after file torrent metadata is acquired and/or
    // resume data has been validated.
    bool _pluginStarted;

    // Tick processor
    void _tick();
    */

    // Checks that peer is not banned and that it is a bittorrent connection
    bool isPeerWellBehaved(libtorrent::peer_connection * connection) const;

    // Send torrent plugin alert to libtorrent session
    void sendTorrentPluginAlert(const TorrentPluginAlert & alert);

private:

    // Use banning of peers
    bool _enableBanningSets;
};

#endif // TORRENT_PLUGIN_HPP
