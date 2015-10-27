/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 26 2015
 */

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <core/controller/PluginInstalled.hpp>
#include <core/controller/TorrentViewModel.hpp>
#include <core/extension/Plugin.hpp>
#include <common/UnspentP2PKHOutput.hpp>
#include <core/controller/Stream.hpp>

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_handle.hpp>
#include <libtorrent/magnet_uri.hpp> // libtorrent::parse_magnet_uri

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>
#include <QTcpServer>
#include <QAbstractSocket> // is nested enum QAbstractSocket::SocketError

class QNetworkAccessManager;
class TorrentStatus;
class TorrentPluginStartedAlert;
class PluginStatusAlert;
class BuyerTorrentPluginStatusAlert;
class SellerTorrentPluginStatusAlert;
class StartedSellerTorrentPlugin;
class StartedBuyerTorrentPlugin;
class SellerPeerAddedAlert;
class BuyerPeerAddedAlert;
class SellerPeerPluginRemovedAlert;
class BuyerPeerPluginRemovedAlert;

namespace Wallet {
    class Manager;
}

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
        //waiting_for_session_to_start,
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
         * Status of Torrent
         */
        enum class Status {

            // Torrent has been libtorrent::async_add_torrent has been called, without response
            being_async_added_to_session,

            // Torrent was successfully added to session.
            // Can be set either from successfull libtorrent::add_torrent_alert,
            // or synchronous adding which succeeds.
            torrent_added_but_not_checked,

            // When a torrent is added, some checking is done
            // - primarily resume data I think, and when that is completed
            // the libtorrent::torrent_checked_alert alert is issued by libtorrent
            torrent_checked,

            /**
            // User has to specify torrent plugin configuration,
            // is set when user dialog is started
            //
            // ** THIS SHOULD POTENTIALLY BE REMOVED, IS TO
            // TIGHTLY COUPLED WITH VIEW DETAILS,
            // FIND BETTER SOLUTION**
            torrent_plugin_configuration_from_user,
            */

            // <== this should probably be removed
            nothing,

            // Result of some pending resume_data() call,
            // can be either
            // libtorrent::save_resume_data_alert
            // libtorrent::save_resume_data_failed_alert
            asked_for_resume_data
        };

        /**
         * This class is for the moment not used, we just use libtorrent::torrent_status fully,
         * but in the future this should change.

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
                         //const libtorrent::torrent_info & torrentInfo
                         const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                         const std::string url);
                         //const TorrentPlugin::Configuration * torrentPluginConfiguration);

            // Constructor from dictionary
            Configuration(const libtorrent::entry::dictionary_type & dictionaryEntry);

            // Destructors
            ~Configuration();

            static Configuration fromTorrentFile(const QString & resource);
            static Configuration fromMagnetLink(const QString & resource);

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

            boost::intrusive_ptr<libtorrent::torrent_info> torrentFile() const;
            //libtorrent::torrent_info torrentInfo() const;
            //void setTorrentInfo(const libtorrent::torrent_info & torrentInfo);

            quint64 flags() const;
            void setFlags(quint64 flags);

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

            // Magnet Link Uri
            std::string _magnetLink;

            // Metadata in torrent file
            // We need pointer since its not copy assignable,
            // hence does not support being set in d
            //
            // Why cant this be const again??
            boost::intrusive_ptr<libtorrent::torrent_info> _torrentFile;

            //libtorrent::torrent_info _torrentInfo;

            // Have to use pointer to get polymorphism. :/
            //const TorrentPlugin::Configuration * _torrentPluginConfiguration;
        };

        // Constructor from members
        Torrent(const libtorrent::sha1_hash & infoHash,
                const std::string & name,
                const std::string & savePath,
                const std::vector<char> & resumeData,
                quint64 flags,
                //const libtorrent::torrent_handle & handle,
                //libtorrent::torrent_info * torrentInfo,
                const boost::intrusive_ptr<libtorrent::torrent_info> & torrentFile,
                Status status);

        // Add plugins
        void addPlugin(const SellerTorrentPlugin::Status & status);
        void addPlugin(const BuyerTorrentPlugin::Status & status);

        // Getters and Setters
        libtorrent::sha1_hash infoHash() const;

        std::string name() const;
        void setName(const std::string & name);

        std::string savePath() const;
        void setSavePath(const std::string & savePath);

        std::vector<char> resumeData() const;
        void setResumeData(const std::vector<char> & resumeData);

        quint64 flags() const;
        void setFlags(quint64 flags);

        //libtorrent::torrent_info * torrentInfo();

        libtorrent::torrent_handle handle() const;
        void setHandle(const libtorrent::torrent_handle & handle);

        Status status() const;
        void setStatus(Status status);

        PluginInstalled pluginInstalled() const;

        TorrentViewModel * model();

        // Stream management
        void addStream(Stream * stream);
        void removeStream(Stream * stream);

        // Given piece was read
        void pieceRead(const boost::shared_array<char> & buffer, int pieceIndex, int size);

        // Given piece was downloaded and checked
        void pieceFinished(int piece);

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

        // Handle to torrent
        // A valid handle is only set after the torrent has been added
        // successfully to session
        libtorrent::torrent_handle _handle;

        // Torrent file
        //libtorrent::torrent_info * _torrentInfo;

        // Status
        Status _status;

        // Plugin currently installed on this torrent
        PluginInstalled _pluginInstalled;

        // Add const pointer to const object of type TorrentPlugin in the future?
        // can be used to look at stuff like plugin mode etc.
        // worth looking at.

        // View model for torrent
        TorrentViewModel _model;

        // All streams for this torrent.
        // Not quite sure if multiple separate streams for one torrent
        // is necessary, if not, then remove this QSet later.
        QSet<Stream *> _streams;
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
                      const std::vector<std::pair<std::string, int> > & dhtRouters,
                      const QVector<Torrent::Configuration> & torrents);

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
         * of port range for running client.
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

        std::vector<std::pair<std::string, int> > getDhtRouters() const;

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

        //// SWITCH TO THIS LATER //////////////////
        // libtorrent::settings_pack settings;
        ///////////////////////////////////////////

        libtorrent::entry _libtorrentSessionSettingsEntry;

        ///////////////////////////////////////////
        // libtorrent::dht_settings dht;
        ///////////////////////////////////////////

        // Listening port range: DO WE EVEN NEED THIS? IT MAY BE PART OF DHT_SETTINGS ENTRY?
        std::pair<int, int> _portRange;

        // Dht routers
        std::vector<std::pair<std::string, int> > _dhtRouters;

        // Torrent configurations
        QVector<Torrent::Configuration> _torrents;
    };

    // Constructor starting session with given state
    Controller(const Configuration & configuration, Wallet::Manager * wallet, QNetworkAccessManager * manager, QLoggingCategory & category);

    // Destructor
    ~Controller();

    /**
     * Libtorrent entry points
     * =================
     * All Q_INVOKABLE routines are not thread safe, and must be invoke through event loop
     * or on same thread as controller owner, the rest should be thread safe.
     */

    // Callback routine called by libtorrent dispatcher routine
    //
    // CRITICAL:
    // Do not under any circumstance have a call to libtorrent in this routine, since the network
    // thread in libtorrent will be making this call, and a new call will result in a dead lock.
    //
    /** THIS ROUTINE MUST NOT BE PUBLICLY VISIBLE IN THE FUTURE **/
    void libtorrent_alert_dispatcher_callback(const std::auto_ptr<libtorrent::alert> & alertAutoPtr);
    void libtorrent_entry_point_alert_notification();

    /**
     * Plugin entry points
     * =================
     * What are terms for calling this again?
     */
     void sellerPeerPluginRemoved(const libtorrent::sha1_hash & infoHash, const libtorrent::tcp::endpoint & endPoint);

     /**
      * Alert processing
      * ==================
      */

     // Process all pending alerts in the libtorrent queue
     //Q_INVOKABLE void processAlertQueue();

     // Process a spesific request
     Q_INVOKABLE void processAlert(const libtorrent::alert * a);

    /**
     * Inspection/Controlling controller
     * =================
     * Routines for inspecting and controlling controller, are not thread safe, so calls have
     * to be on same thread as owner of controller.
     * Typical caller is view or test code.
     */

    // Manage torrents
    bool addTorrent(const Torrent::Configuration & configuration);
    bool addTorrent(const Torrent::Configuration & configuration, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    bool addTorrent(const Torrent::Configuration & configuration, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo);
    //bool addTorrent(const Torrent::Configuration & configuration, const ObserverTorrentPlugin::Configuration & pluginConfiguration);

    // Start torrent plugin
    //void startTorrentPlugin(const libtorrent::sha1_hash & info_hash, const TorrentPlugin::Configuration * configuration);
    void startSellerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const SellerTorrentPlugin::Configuration & pluginConfiguration);
    void startBuyerTorrentPlugin(const libtorrent::sha1_hash & info_hash, const BuyerTorrentPlugin::Configuration & pluginConfiguration, const Coin::UnspentP2PKHOutput & utxo);
    //void startObserverTorrentPlugin(const libtorrent::sha1_hash & info_hash, const ObserverTorrentPlugin::Configuration & pluginConfiguration);

    // Stops libtorrent session, and tries to save_resume data, when all resume data is saved, finalize_close() is called.
    void begin_close();

    // Returns torrent handle for torrent with give info hash, if no such torrent has been registered
    // then an invalid handle is passed
    //libtorrent::torrent_handle getTorrentHandle(const libtorrent::sha1_hash & infoHash) const;

    /**
     * Stram management stuff
     * THIS ROUTINE MUST BE HIDDEN FROM CONTROLLER USER IN THE FUTURE
     **/

    // If there is a torrent for the given info hash, then the given stream is added to the
    // torrents streams set, and lastly a handle for the torrent is removed. Otherwise,
    // same rules as getTorrentHandle apply.
    libtorrent::torrent_handle registerStream(Stream * stream);

    // Removes stream registration
    void unRegisterStream(Stream * stream);
    void unRegisterStream(Stream * stream, Stream::Error error);

    // If torrent corresponding to the given info hash has a buyer torrent plugin installed,
    // then the plugin is requested to alter download location
    void changeDownloadingLocationFromThisPiece(const libtorrent::sha1_hash & infoHash, int pieceIndex);

    // Returns port server is presently listening on
    quint16 getServerPort() const;

    // Returns reference to the wallet
    Wallet::Manager * wallet();

    // Get view model for given torrent
    const TorrentViewModel * torrentViewModel(const libtorrent::sha1_hash & infoHash) const;

    // Save state of controller
    Configuration configuration() const;

public slots:

    /**
     * View entry points
     * =================
     * Are not thread safe. Primarily called by view objects on the same thread as controller thread,
     * buy also good routines to use for testing.
     */

    void pauseTorrent(const libtorrent::sha1_hash & info_hash);
    void startTorrent(const libtorrent::sha1_hash & info_hash);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

private slots:

    /**
     * Intra Controller entry points
     * =================
     * Typically parts of controller,
     * remove this at a later time
     */

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();

    /**
     * Handlers for TCP streamng server
     */

    // Checks server for pending connections
    void handleConnection();

    // Checks server for pending error
    void handleAcceptError(QAbstractSocket::SocketError socketError);

    /**
    // Streaming server signals
    void registerStream(const Stream * handler);
    void handleFailedStreamCreation(QAbstractSocket::SocketError socketError);

    // Stream signal
    void registerRequestedPathOnStream(const Stream * handler, const QByteArray & requestedPath) const;

    // Will ask libtorrent to read given piece
    void readPiece(int piece);
    */

signals:

    // Sent when libtorrent::add_torrent_alert is received from libtorrent
    void addedTorrent(const TorrentViewModel * model);

    // Torrent with given info hash was removed
    void torrentRemoved(const libtorrent::sha1_hash & info_hash);

    // A torrent was not added successfully according to libtorrent session giving
    // a libtorrent::add_torrent_alert p->error was done.
    void failedToAddTorrent(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);

    // A torrent plugin was started
    // *** DISCONTINUED, THESE ARE INSTALLED AS SIGNALS ON TORRENT VIEW MODELS
    //void torrentPluginStarted();
    //void startedSellerTorrentPlugin(SellerTorrentPluginViewModel * model);
    //void startedBuyerTorrentPlugin(BuyerTorrentPluginViewModel * model);

    // Notify view
    void torrentCheckedButHasNoPlugin(const libtorrent::torrent_info & torrentInfo, const libtorrent::torrent_status & torrentStatus);

    // Status update from underlying libtorrent session
    void pluginStatusUpdate(const Plugin::Status & status);

    // Emitted after finalize_close(), that is when controller is 100% done
    void closed();

private:

    // State of controller
    State _state;
	
    // Underlying libtorrent session,
    // has to be pointer since it needs sessings_pack,
    // which can only be built postfix calls
    libtorrent::session * _session;

    // Wallet used
    Wallet::Manager * _wallet;

    // Logging category
    QLoggingCategory & _category;

    // Network access manager reference
    // *** REMOVE
    // *** REMOVE
    // *** IS THERE ANY REASON FOR
    QNetworkAccessManager * _manager;

    // Plugin: constructor initializatin list expects plugin to appear after category_

    // Should this be boost::shared_ptr<Plugin>, since life time of object is managed by it?
    // on the other hand, we loose Plugin behaviour through libtorrent::plugin pointer, which we need!
    Plugin * _plugin;

    // Port range for which libtorrent should try to listen
    // Must persist so that it can be saved in controller contiguration
    // upon closing
    std::pair<int, int> _portRange;

    // Timer which calls session.post_torrent_updates() at regular intervals
    QTimer _statusUpdateTimer;

    // Torrents added to session
    // Has to be pointer since since its Torrent::model (TorrentViewModel) isQObject type.
    // Object is entirely owned by this.
    QMap<libtorrent::sha1_hash, Torrent *> _torrents;

    // TCP streaming server
    QTcpServer _streamingServer;

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
    QMap<libtorrent::sha1_hash, QPair<BuyerTorrentPlugin::Configuration, Coin::UnspentP2PKHOutput> > _pendingBuyerTorrentPluginConfigurationAndUtxos;
    //QMap<libtorrent::sha1_hash, UnspentP2PKHOutput> _pendingBuyerTorrentPluginUtxos;
    //QMap<libtorrent::sha1_hash, ObserverTorrentPlugin::Configuration> _pendingObserverTorrentPluginConfigurations;

    // Routine for processig libtorrent alerts
    void processMetadataReceivedAlert(libtorrent::metadata_received_alert const * p);
    void processMetadataFailedAlert(libtorrent::metadata_failed_alert const * p);
    void processListenFailedAlert(libtorrent::listen_failed_alert const * p);
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processTorrentFinishedAlert(libtorrent::torrent_finished_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);
    void processTorrentCheckedAlert(libtorrent::torrent_checked_alert const * p);
    void processReadPieceAlert(const libtorrent::read_piece_alert * p);
    void processPieceFinishedAlert(const libtorrent::piece_finished_alert * p);

    //void processTorrentPluginStartedAlert(const TorrentPluginStartedAlert * p);
    void processStartedSellerTorrentPlugin(const StartedSellerTorrentPlugin * p);
    void processStartedBuyerTorrentPlugin(const StartedBuyerTorrentPlugin * p);

    //void processTorrentPluginStatusAlert(const TorrentPluginStatusAlert * p);
    void processBuyerTorrentPluginStatusAlert(const BuyerTorrentPluginStatusAlert * p);
    void processSellerTorrentPluginStatusAlert(const SellerTorrentPluginStatusAlert * p);

    void processPluginStatusAlert(const PluginStatusAlert * p);

    void processSellerPeerAddedAlert(const SellerPeerAddedAlert * p);
    void processBuyerPeerAddedAlert(const BuyerPeerAddedAlert * p);

    void processSellerPeerPluginRemovedAlert(const SellerPeerPluginRemovedAlert * p);
    void processBuyerPeerPluginRemovedAlert(const BuyerPeerPluginRemovedAlert * p);

    // Status
    void update(const std::vector<libtorrent::torrent_status> & statuses);
    void update(const libtorrent::torrent_status & status);

    // Start torrent plugin
    void startTorrentPlugin(const libtorrent::sha1_hash & info_hash);

    // Tell libtorrent try save resume data for all torrents needing it
    int makeResumeDataCallsForAllTorrents();

    // Routine called after all resume data has been saved as part of an initlal begin_close() call
    void finalize_close();

    // Save state of controller to file
    void saveStateToFile(const char * file);
};

#endif // CONTROLLER_HPP
