#ifndef CONTROLLER_H
#define CONTROLLER_H

//#include "ControllerConfiguration.hpp"
#include "PluginInstalled.hpp"
#include "view/MainWindow.hpp"
#include "extension/Plugin.hpp"
//#include "extension/PeerPluginStatus.hpp" // needed for QT moc <==== Remove later
#include "extension/BitCoin/Wallet.hpp"
#include "extension/BitCoin/UnspentP2PKHOutput.hpp"

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>
#include <QNetworkAccessManager>

class TorrentStatus;
//class TorrentPluginStatusAlert;
class BuyerTorrentPluginStatusAlert;
class SellerTorrentPluginStatusAlert;
class StartedSellerTorrentPlugin;
class StartedBuyerTorrentPlugin;


class TorrentPluginStartedAlert;
class PluginStatusAlert;

namespace libtorrent {
    class peer_connection;
}

class Controller : public QObject {

    Q_OBJECT

public:

    /**
     * @brief
     */
    enum class State {
        normal,
        waiting_for_resume_data_while_closing
    };

    /**
     * @brief Torrent as seen by the controller.
     */
    class Torrent {

    public:

        /**
         * @brief The State enum
         *
         * Async pending states of this torrent
         */
        enum class ExpectedEvent {

            // libtorrent::add_torrent_alert,
            // which may indicate sucess or failure
            torrent_added_alert,

            // libtorrent::torrent_checked_alert,
            // When a torrent is added, some checking is done
            // - primarily resume data I think, and when that is completed
            // the given alert is issued by libtorrent
            torrent_checked_alert,

            // User has to specify torrent plugin configuration,
            // is set when user dialog is started
            torrent_plugin_configuration_from_user,

            //
            nothing,

            // Result of some pending resume_data() call,
            // can be either
            // libtorrent::save_resume_data_alert
            // libtorrent::save_resume_data_failed_alert
            save_resume_data_alert
        };

        /**
         * @brief The Status class

        class Status {

        public:

        private:

            libtorrent::torrent_status::state_t _state;

            float _progress;

            // Download rate (bytes/s)
            int _downloadRate;

            // Upload rate (bytes/s)
            int _uploadRate;

            // Total number of peers connected to torrent
            int _numberOfPeers;

            // Total number of peers with extension
            int _numberOfPeersWithExtension;

            // Plugin currently installed on this torrent
            PluginInstalled pluginInstalled;


        };
        */

        /**
         * @brief Persistant state of Torrent.
         */
        class Configuration {

        public:

            // Default constructor
            Configuration();

            // Constructor from members
            Configuration(const libtorrent::sha1_hash & infoHash,
                         const std::string & name,
                         const std::string & savePath,
                         const std::vector<char> & resumeData,
                         quint64 flags,
                         libtorrent::torrent_info * torrentInfo);
                         //const TorrentPlugin::Configuration * torrentPluginConfiguration);

            // Constructor from dictionary
            Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

            // Destructors
            ~Configuration();

            /**
             * Write state into dictionary
             * ===============================================================
             *
             * Torrent parameters as they persist across sessions on disk
             * encoded as entry::dictionary_type with the following keys:
             *
             * "infoHash" -> entry::string_type object representing info hash of torrent.
             *
             * "name" -> entry::string_type object representing name of torrent.
             *
             * "savePath" -> entry::string_type object representing save_path parameter.
             *
             * "resumeData" -> entry::string_type object representing resume data.
             *
             * "flags" -> entry::integer_type object representing flag parameter.
             *
             * "torrentInfo" -> not implemented
             *
             ///* "torrentPluginConfiguration" -> entry::dictionary_type object representing _torrentPluginConfiguration as encoded by TorrentPluginConfiguration::toDictionaryEntry().
             */
            void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

            // Turn into add_torrent_params
            // will allocate new memory for resume_data pointer, which is owned by caller
            libtorrent::add_torrent_params toAddTorrentParams() const;

            // Getters

            //const TorrentPlugin::Configuration * torrentPluginConfiguration() const;
            //void setTorrentPluginConfiguration(const TorrentPlugin::Configuration *torrentPluginConfiguration);

            libtorrent::torrent_info * torrentInfo() const;
            void setTorrentInfo(libtorrent::torrent_info * torrentInfo);

            quint64 flags() const;
            void setFlags(const quint64 & flags);

            std::vector<char> resumeData() const;
            void setResumeData(const std::vector<char> & resumeData);

            std::string savePath() const;
            void setSavePath(const std::string & savePath);

            std::string name() const;
            void setName(const std::string & name);

            libtorrent::sha1_hash infoHash() const;
            void setInfoHash(const libtorrent::sha1_hash & infoHash);

        private:

            // Info hash of torrent
            libtorrent::sha1_hash _infoHash;

            // Name of torrent
            std::string _name; // how should this be used?

            // Save path
            std::string _savePath;

            // Resume data
            std::vector<char> _resumeData;

            // Flags
            quint64 _flags;

            // Metadata in torrent file
            // We need pointer since we cannot copy torrent_info
            libtorrent::torrent_info * _torrentInfo;

            // Have to use pointer to get polymorphism. :/
            //const TorrentPlugin::Configuration * _torrentPluginConfiguration;
        };

        // Default constructor
        Torrent();

        // Constructor from members
        Torrent(const libtorrent::sha1_hash & infoHash,
                const std::string & name,
                const std::string & savePath,
                const std::vector<char> & resumeData,
                quint64 flags,
                libtorrent::torrent_info * torrentInfo,
                ExpectedEvent event,
                PluginInstalled pluginInstalled);

        // Getters and Setters
        libtorrent::sha1_hash infoHash() const;
        void setInfoHash(const libtorrent::sha1_hash &infoHash);

        std::string name() const;
        void setName(const std::string &name);

        std::string savePath() const;
        void setSavePath(const std::string &savePath);

        std::vector<char> resumeData() const;
        void setResumeData(const std::vector<char> &resumeData);

        quint64 flags() const;
        void setFlags(const quint64 &flags);

        libtorrent::torrent_info *torrentInfo() const;
        void setTorrentInfo(libtorrent::torrent_info *torrentInfo);

        ExpectedEvent event() const;
        void setEvent(const ExpectedEvent &event);

        PluginInstalled pluginInstalled() const;
        void setPluginInstalled(PluginInstalled pluginInstalled);

    private:

        // Info hash of torrent
        libtorrent::sha1_hash _infoHash;

        // Name of torrent
        std::string _name; // how should this be used?

        // Save path
        std::string _savePath;

        // Resume data
        std::vector<char> _resumeData;

        // Flags
        quint64 _flags;

        // Torrent file
        libtorrent::torrent_info * _torrentInfo;

        // Next expected event for this torrent
        ExpectedEvent _event;

        // Plugin currently installed on this torrent
        PluginInstalled _pluginInstalled;

        // Add const pointer to const object of type TorrentPlugin in the future?
        // can be used to look at stuff like plugin mode etc.
        // worth looking at.
    };

    /**
     * @brief Persistant state of Controller.
     */
    class Configuration {

    public:

        // Constructor using default parameter settings
        Configuration();

        // Destructor
        ~Configuration();

        // Constructor using members
        Configuration(const libtorrent::entry & libtorrentSessionSettingsEntry,
                      const std::pair<int, int> & portRange,
                      const std::vector<std::pair<std::string, int>> & dhtRouters,
                      const QVector<Torrent::Configuration> & torrents,
                      const QString walletFile);

        // Constructor using dictionary entry
        Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

        // Constructor using file
        Configuration(const char * fileName);

        /**
         * Saves to dictionary entry
         * ===============================================================
         *
         * Controller parameters as they persist across sessions on disk
         * encoded as entry::dictionary_type with the following keys:
         *
         * "libtorrentSettings" -> entry::dictionary_type object from session.save_state().
         *
         * "portRange" -> entry::list_type object with two positive integers used as start (first) and end (second)
         * of port range for running bitswapr.
         *
         * "dhtRouters" -> entry::list_type object with entry::list_type objects with two elements, each encoding a dht router by the host (first)
         * and port (second).
         *
         * "torrentConfigurations" -> entry::list_type object, with list item objects being of type entry::dictionary_type and
         * representing state of corresponding torrent as dictated by encoding used in TorrentConfiguration::toDictionaryEntry().
         */
        void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry);

        // Saves to file
        void saveToFile(const char * file);

        /*
        // Inserts torrent configuration.
        // This object then takes ownership of file, and deletes in constructor
        void insertTorrentConfiguration(TorrentConfiguration * torrentConfiguration);
        */

        // Getters & Setters
        libtorrent::entry getLibtorrentSessionSettingsEntry() const;
        void setLibtorrentSessionSettingsEntry(const libtorrent::entry & libtorrentSessionSettingsEntry);

        std::pair<int, int> getPortRange() const;

        std::vector<std::pair<std::string, int>> getDhtRouters() const;

        //std::vector<TorrentConfiguration *>::const_iterator getBeginTorrentConfigurationsIterator() const;
        //std::vector<TorrentConfiguration *>::const_iterator getEndTorrentConfigurationsIterator() const;


        /*
        QMap<libtorrent::sha1_hash, QPair<Torrent::Configuration, BuyerTorrentPlugin::Configuration> > buyers() const;
        void setBuyers(const QMap<libtorrent::sha1_hash, QPair<Torrent::Configuration, BuyerTorrentPlugin::Configuration> > & buyers);

        QMap<libtorrent::sha1_hash, QPair<Torrent::Configuration, SellerTorrentPlugin::Configuration> > sellers() const;
        void setSellers(const QMap<libtorrent::sha1_hash, QPair<Torrent::Configuration, SellerTorrentPlugin::Configuration> > & sellers);
        */

        //TorrentPlugin::Configuration *getTorrentPluginConfiguration() const;
        //void setTorrentPluginConfiguration(TorrentPlugin::Configuration *value);

        QVector<Torrent::Configuration> torrents() const;
        void setTorrents(const QVector<Torrent::Configuration> & torrents);

        libtorrent::sha1_hash infoHash() const;
        void setInfoHash(const libtorrent::sha1_hash &infoHash);

        std::string name() const;
        void setName(const std::string &name);

        std::string savePath() const;
        void setSavePath(const std::string &savePath);

        std::vector<char> resumeData() const;
        void setResumeData(const std::vector<char> &resumeData);

        quint64 flags() const;
        void setFlags(const quint64 &flags);

        libtorrent::torrent_info *torrentInfo() const;
        void setTorrentInfo(libtorrent::torrent_info *torrentInfo);

        //const TorrentPlugin::Configuration *torrentPluginConfiguration() const;
        //void setTorrentPluginConfiguration(const TorrentPlugin::Configuration *torrentPluginConfiguration);

        QString walletFile() const;
        void setWalletFile(const QString &walletFile);

    private:

        /*
         * Holds all settings of session, that includes
         * session_settings, dht_settings, dht_state,
         * proxy_settings, i2p_proxy, pe_settings, feed
         * and extension settings.
         *
         * It would be cleaner to save class representations rather
         * than using entry type, however that gets messy and/or
         * requires lots of extra work for various reasons.
         */
        libtorrent::entry _libtorrentSessionSettingsEntry;

        // Listening port range: DO WE EVEN NEED THIS? IT MAY BE PART OF DHT_SETTINGS ENTRY?
        std::pair<int, int> _portRange;

        // Dht routers
        std::vector<std::pair<std::string, int>> _dhtRouters;

        // Torrent configurations
        QVector<Torrent::Configuration> _torrents;

        // Location of wallet file
        QString _walletFile;

    };

    // Constructor starting session with given state
    Controller(const Configuration & configuration, bool showView, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category);

    // Callback routine called by libtorrent dispatcher routine
    void libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr);

    // Invocations of this method are queued, and dispatcher callback
    Q_INVOKABLE void processAlert(libtorrent::alert const * a);

    /**
     * View entry points
     * =================
     * Primarily called by view objects on the same thread as controller thread,
     * buy also good routines to use for testing.
     */

    // Manage torrents
    bool addTorrent(const Torrent::Configuration & configuration);
    bool addTorrent(const Torrent::Configuration & configuration, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    bool addTorrent(const Torrent::Configuration & configuration, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const UnspentP2PKHOutput & utxo);
    //bool addTorrent(const Torrent::Configuration & configuration, const ObserverTorrentPlugin::Configuration & pluginConfiguration);

    // Start torrent plugin
    //void startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration);
    void startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    void startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const UnspentP2PKHOutput & utxo);
    //void startObserverTorrentPlugin(const libtorrent::sha1_hash & info_hash, const ObserverTorrentPlugin::Configuration & pluginConfiguration);


    // Stops libtorrent session, and tries to save_resume data, when all resume data is saved, finalize_close() is called.
    void begin_close();

    // Returns reference to the wallet
    Wallet & wallet();

    // Save state of controller
    Configuration configuration() const;

public slots:

    /**
     * View entry points
     * =================
     * Primarily called by view objects on the same thread as controller thread,
     * buy also good routines to use for testing.
     */

    void pauseTorrent(const libtorrent::sha1_hash & info_hash);
    void startTorrent(const libtorrent::sha1_hash & info_hash);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

    void showSellerTorrentPluginDialog(const SellerTorrentPluginViewModel * model);
    void showBuyerTorrentPluginDialog(const BuyerTorrentPluginViewModel * model);

private slots:

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();

signals:

    // Emitted after finalize_close(), that is when controller is 100% done
    void closed();

private:

    // State of controller
    State _state;
	
    // Underlying libtorrent session
    libtorrent::session _session;

    // Wallet used
    Wallet _wallet;

    // Logging category
    QLoggingCategory & _category;

    // Network access manager reference
    QNetworkAccessManager & _manager;

    // Plugin: constructor initializatin list expects plugin to appear after category_

    // Should this be boost::shared_ptr<Plugin>, since life time of object is managed by it?
    // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
    Plugin * _plugin;

    // Port range for which libtorrent should try to listen
    // Must persist so that it can be saved in controller contiguration
    // upon closing
    std::pair<int, int> _portRange;

    // View
    MainWindow _view;

    // Timer which calls session.post_torrent_updates() at regular intervals
    QTimer _statusUpdateTimer;

    // Torrents added to session
    QMap<libtorrent::sha1_hash, Torrent> _torrents;

    /**
     *
     * SIMPLIFY LATER: Put TorrentPlugin::Configuration pointer into Torrent.
     * Do not refer to it as *pending*, this will bethe configurations the
     * view is given when any editing of configruations is done, and
     * which will be pased on to plugin as alert.
     *
     * In the case of buyer, put utxo in QMap for keeping pending utxo. They
     * are indeed pending, and are therefore not part of Torrent.
     *
     *
     */
    // Configurations are placed in these maps when corresponding torrent is added to session,
    // and they are used to start a plugin on the given torrent once a torrent_checked_alert has been
    // issued by session.
    //QMap<libtorrent::sha1_hash, const TorrentPlugin::Configuration *> _pendingConfigurations;
    QMap<libtorrent::sha1_hash, SellerTorrentPlugin::Configuration> _pendingSellerTorrentPluginConfigurations;
    QMap<libtorrent::sha1_hash, QPair<BuyerTorrentPlugin::Configuration, UnspentP2PKHOutput> > _pendingBuyerTorrentPluginConfigurationAndUtxos;
    //QMap<libtorrent::sha1_hash, UnspentP2PKHOutput> _pendingBuyerTorrentPluginUtxos;
    //QMap<libtorrent::sha1_hash, ObserverTorrentPlugin::Configuration> _pendingObserverTorrentPluginConfigurations;

    // Routine for processig libtorrent alerts
    void processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p);
    void processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p);
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);
    void processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p);

    //void processTorrentPluginStartedAlert(const TorrentPluginStartedAlert * p);
    void processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p);
    void processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p);

    //void processTorrentPluginStatusAlert(const TorrentPluginStatusAlert * p);
    void processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p);
    void processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p);

    void processPluginStatusAlert(const PluginStatusAlert * p);

    // Start torrent plugin
    void startTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Tell libtorrent try save resume data for all torrents needing it
    int makeResumeDataCallsForAllTorrents();

    // Routine called after all resume data has been saved as part of an initlal begin_close() call
    void finalize_close();

    // Save state of controller to file
    void saveStateToFile(const char * file);
};

#endif
