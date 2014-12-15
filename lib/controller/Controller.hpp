#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ControllerConfiguration.hpp"
#include "view/MainWindow.hpp"
#include "extension/Plugin.hpp"
#include "extension/TorrentPluginStatus.hpp" // needed for QT moc
#include "extension/PeerPluginStatus.hpp" // needed for QT moc

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>
#include <QMutex>


class libtorrent::peer_connection;

class Controller : public QObject {

    Q_OBJECT

private:
	
    // Underlying libtorrent session
    libtorrent::session _session;

    // Persistent state of controller
    ControllerConfiguration _controllerConfiguration;

    /**
     * Routine for processig libtorrent alerts
     */
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);

    // Tell libtorrent try save resume data for all torrents needing it
    int makeResumeDataCallsForAllTorrents();

    // Routine called after all resume data has been saved as part of an initlal begin_close() call
    void finalize_close();

    // Creates folder and adds torrent to session, but does not alter state of controller by adding torrent configuration
    // to it. Is typically called by ctr() and addTorrent().
    bool addTorrentToSession(const TorrentConfiguration & TorrentConfiguration);

    // Save state of controller to file
    void saveStateToFile(const char * file);

   /**
    * Private short term state
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

    // Different sources for a resume data call
    enum sourceForLastResumeDataCallType {
        NONE,
        CLIENT_PAUSE,
        TORRENT_PAUSE,
        CLIENT_CLOSE
    };

    // Actual source of resume data call
    sourceForLastResumeDataCallType _sourceForLastResumeDataCall;

    // Timer which calls session.post_torrent_updates() at regular intervals
    QTimer _statusUpdateTimer;

    // Logging category
    QLoggingCategory & _category;

    // Plugin: constructor initializatin list expects plugin to appear after category_
    boost::shared_ptr<libtorrent::plugin> _plugin; // should this be weak

    // View
    MainWindow _view;

    /**
      * Mutexes used to synchronize access to critical data in the controller,
      * so that the libtorrent thread can make calls into the public entry points
      * in the controller.
      */

    // SHOULD THIS LOCK BE PLACES INSIDE CONTROLLER CONFIGURATION
    // CLASS, AND THEN PUT SYNCHRONIZED MANIPULATION ROUTINES (ADD,REMOVE..)
    // INSIDE THE CLASS.
    QMutex _controllerConfigurationMutex;

public:

	// Constructor starting session with given state
    Controller(const ControllerConfiguration & controllerConfiguration, bool showView, QLoggingCategory & category);

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
    bool addTorrent(const TorrentConfiguration & TorrentConfiguration);
    bool removeTorrent(const libtorrent::sha1_hash & info_hash);
    bool pauseTorrent(const libtorrent::sha1_hash & info_hash);
    bool startTorrent(const libtorrent::sha1_hash & info_hash);

    // Stops libtorrent session, and tries to save_resume data, when all resume data is saved, finalize_close() is called.
    void begin_close();

    /**
      * Libtorrent entry points
      * =======================
      * All public routines below are entry points for libtorrent
      * thread, and all use locks to synchronize access to various parts of the
      * controller state. The routines are used for synchronous calls to the controller,
      * for asynchronous calls uses slots.
      */

    // Caller does not own object, life time is uncertain, at least make copy.
    // Do something safer later?
    //const TorrentPluginConfiguration * getTorrentPluginConfiguration(const libtorrent::sha1_hash & info_hash);

private slots:

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();

public slots:

    /*
    // Primarily used by plugin routines, run by libtorrent thread.
    void addPeerPlugin(libtorrent::tcp::endpoint endPoint);
    void updateTorrentPluginStatus(TorrentPluginStatus status);
    void updatePeerPluginStatus(PeerPluginStatus status);
    void removePeerPlugin(libtorrent::tcp::endpoint endPoint);
    */

signals:

    // Emitted after finalize_close(), that is when controller is 100% done
    void closed();
};

#endif
