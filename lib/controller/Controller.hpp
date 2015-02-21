#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ControllerConfiguration.hpp"
#include "view/MainWindow.hpp"
#include "extension/Plugin.hpp"
#include "extension/PeerPluginStatus.hpp" // needed for QT moc <==== Remove later

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
class TorrentPluginStatusAlert;
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

            // Added to session, but not yet checked
            torrent_checked_alert,

            // User has to specify torrent plugin configuration
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
             * "torrentPluginConfiguration" -> entry::dictionary_type object representing _torrentPluginConfiguration as encoded by TorrentPluginConfiguration::toDictionaryEntry().
             */
            void toDictionaryEntry(libtorrent::entry::dictionary_type & dictionaryEntry) const;

            // Turn into add_torrent_params
            // will allocate new memory for resume_data pointer, which is owned by caller
            libtorrent::add_torrent_params toAddTorrentParams() const;

            // Getters
            libtorrent::sha1_hash getInfoHash() const;
            std::string getName() const;
            std::string getSavePath() const;
            std::vector<char> getResumeData() const;
            quint64 getFlags() const;
            const libtorrent::torrent_info * getTorrentInfo() const;

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

            /**




              Should TorrentPlugin::Configuration go here, and should it
              be ptr, or should we subclass with different corresponding types


              */



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
                const TorrentPlugin::Configuration * loadedTorrentPluginConfiguration,
                ExpectedEvent event);

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

        // The torrent plugin configuration which was loaded from disk,
        // we need to keep it arround since we may only add it once the
        // libtorrent::torrent_checked_alert is received from libtorrent.
        const TorrentPlugin::Configuration * _loadedTorrentPluginConfiguration;

        // Next expected event for this torrent
        ExpectedEvent _event;

        // Add const pointer to const object of type TorrentPlugin in the future?
        // can be used to look at stuff like plugin mode etc.
        // worth looking at.
    };

    /**
     * @brief Persistant state of Controller.
     */
    class Configuration {

    public:

    private:

        // Map to info_hash to persistant state of torrents
        QMap<libtorrent::sha1_hash, Torrent::Configuration> _torrentConfigurations;

    };

    // Constructor starting session with given state
    Controller(const ControllerConfiguration & controllerConfiguration, bool showView, QNetworkAccessManager & manager, QString bitcoindAccount, QLoggingCategory & category);

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
    bool addTorrent(const Torrent::Configuration & configuration, bool promptUserForTorrentPluginConfiguration);
    bool removeTorrent(const libtorrent::sha1_hash & info_hash);
    bool pauseTorrent(const libtorrent::sha1_hash & info_hash);
    bool startTorrent(const libtorrent::sha1_hash & info_hash);
    void updateTorrentPluginConfiguration(const libtorrent::sha1_hash & infoHash, TorrentPlugin::Configuration * configuration);

    // Stops libtorrent session, and tries to save_resume data, when all resume data is saved, finalize_close() is called.
    void begin_close();

    // Save state of controller
    ControllerConfiguration toControllerConfiguration() const;

private:
	
    // Underlying libtorrent session
    libtorrent::session _session;

    // Logging category
    QLoggingCategory & _category;

    // Network access manager reference
    QNetworkAccessManager & _manager;

    // Plugin: constructor initializatin list expects plugin to appear after category_
    // should this be weak?
    Plugin * _plugin;

    // Port range for which libtorrent should try to listen
    // Must persist so that it can be saved in controller contiguration
    // upon closing
    std::pair<int, int> _portRange;

    // View
    MainWindow _view;

    // Timer which calls session.post_torrent_updates() at regular intervals
    QTimer _statusUpdateTimer;

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
    void processTorrentPluginStatusAlert(const TorrentPluginStatusAlert * p);
    void processPluginStatusAlert(const PluginStatusAlert * p);

    // Tell libtorrent try save resume data for all torrents needing it
    int makeResumeDataCallsForAllTorrents();

    // Routine called after all resume data has been saved as part of an initlal begin_close() call
    void finalize_close();

    // Save state of controller to file
    void saveStateToFile(const char * file);

   /**
    * Short term controller state
    * ==========================
    * Needed for processing which relies
    * on asyncrhoous calls to libtorrent. It is short term in the sense
    * that it does not need to persist across client sessions.
    *
    * Represents the number of calls that have been made to save resume data,
    * for which a save_resume_data_failed_alert/save_resume_data_alert has not been
    * received. If this number is greater than one, then no new (series) of calls to save resume
    * data should be made, because that will confuse source of call, which can be
    * (a) closing client
    * (b) pausing client
    * (c) pausing an individual torrent
    */

    unsigned int _numberOfOutstandingResumeDataCalls;

    /**
     * ABSORB INTO TORRENT TYPE LATER
     * ==============================
     */

    // Different sources for a resume data call
    enum sourceForLastResumeDataCallType {
        NONE,
        CLIENT_PAUSE,
        TORRENT_PAUSE,
        CLIENT_CLOSE
    };

    // Actual source of resume data call
    sourceForLastResumeDataCallType _sourceForLastResumeDataCall;

    // Torrents added to session
    QMap<libtorrent::sha1_hash, Torrent> _torrents;

private slots:

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();

signals:

    // Emitted after finalize_close(), that is when controller is 100% done
    void closed();
};

#endif
