#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ControllerState.hpp"
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

#define CLIENT_FINGERPRINT "BR"

// Register types for signal and slots
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)
Q_DECLARE_METATYPE(const libtorrent::alert*) // Register type for QMetaObject::invokeMethod

class libtorrent::peer_connection;

class Controller : public QObject {

    Q_OBJECT

private:
	
	/*
	* Underlying libtorrent session,
	* it is made public through get routine getSession()
	* so view thread can control session
	* since this is mostly thread safe
	* MAY need to be moved later.
    */
	libtorrent::session session;

    // Listening port range : MOVE LATER
	std::pair<int, int> portRange;

    // Parameters for all torrents added: WRAP IN PROPER CLASS LATER
	std::vector<libtorrent::add_torrent_params> addTorrentParameters;

    // Dht routers: MOVE LATER
	std::vector<std::pair<std::string, int>> dhtRouters;

    // Timer which calls session.post_torrent_updates() at regular intervals
    QTimer statusUpdateTimer;

    // Routine for processig libtorrent alerts
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);

    // THIS HAS TO GO LATER! AND GET RID OF AUTO_PTR
    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> findTorrentParamsFromInfoHash(const libtorrent::sha1_hash & info_hash);

    bool loadResumeDataForTorrent(libtorrent::add_torrent_params & params) const;
    //bool loadResumeDataForTorrent(QString const & save_path, QString const & file_name, std::vector<char> * resume_data) const;

    // Tell libtorrent try save resume data for all torrents needing it
    int makeResumeDataCallsForAllTorrents();

    // Tries to save the resume data for a torrent
    bool saveResumeDataForTorrent(QString const & save_path, QString const & file_name, std::vector<char> const & resume_data) const;

    QString resumeFileNameForTorrent(libtorrent::sha1_hash & info_hash) const;

    // Routine called after all resume data has been saved
    void finalize_close();

    // Private short term state:
    // Needed for processing which relies
    // on asyncrhoous calls to libtorrent. It is short term in the sense
    // that it does not need to persist across client sessions.

    // Represents the number of calls that have been made to save resume data,
    // for which a save_resume_data_failed_alert/save_resume_data_alert has not been
    // received. If this number is greater than one, then no new (series) of calls to save resume
    // data should be made, because that will confuse source of call, which can be
    // a) closing client
    // b) pausing client
    // c) pausing an individual torrent
    unsigned int numberOfOutstandingResumeDataCalls;

    // Different sources for a resume data call
    enum sourceForLastResumeDataCallType {
        NONE,
        CLIENT_PAUSE,
        TORRENT_PAUSE,
        CLIENT_CLOSE
    };

    // Actual source of resume data call
    sourceForLastResumeDataCallType sourceForLastResumeDataCall;

    // Logging category
    QLoggingCategory & category_;

    // Plugin: constructor initializatin list expects plugin to appear after category_
    boost::shared_ptr<libtorrent::plugin> plugin;

    // View
    MainWindow view;

public:

	// Constructor starting session with given state
    Controller(const ControllerState & state, bool showView, QLoggingCategory & category);

    // Callback routine called by libtorrent dispatcher routine
    void libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr);

    // Invocations of this method are queued, and dispatcher callback
    Q_INVOKABLE void processAlert(libtorrent::alert const * a);

    /*
     * Utilities
     */

    // Called by MainWindow::
    libtorrent::torrent_handle getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash);

    /*
     * View/Main entry points.
     * TURN INTO SLOTS LATER
     */

	void saveStateToFile(const char * file);

    // REMOVE LATER, not nice to share this out.
    libtorrent::session & getSession();

    // Called by AddTorrentDialog::on_AddTorrentDialog_accepted()
    Q_INVOKABLE void addTorrent(libtorrent::add_torrent_params & params);

    // Called by MainWindow::on_addTorrentFilePushButton_clicked()
    void addTorrentFromTorrentFile(const QString & torrentFile);

    // Called by MainWindow::on_
    void addTorrentFromMagnetLink(const QString & magnetLink);

    // Called by MainWindow::startMenuAction()
    void removeTorrent(const libtorrent::torrent_handle & torrentHandle);

    // Called by MainWindow::pauseMenuAction()
    void pauseTorrent(libtorrent::torrent_handle & torrentHandle);

    // Called by MainWindow::startMenuAction()
    void startTorrent(libtorrent::torrent_handle & torrentHandle);

    // Called by
    // Stops libtorrent session, and tries to save_resume data.
    // When all resume data is saved, finalize_close() is called.
    void begin_close();

private slots:

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();

public slots:

    /*
     * Primarily used by plugin routines, run by libtorrent thread.
     */

    void extensionPeerAdded(PeerPlugin * peerPlugin);
    void updateTorrentPluginStatus(TorrentPluginStatus status);
    void updatePeerPluginStatus(PeerPluginStatus status);

signals:

    // Emitted after finalize_close(), that is when controller is 100% done
    void closed();
};

#endif
