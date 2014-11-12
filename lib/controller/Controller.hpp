#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller/ControllerState.hpp"
#include "view/mainwindow.hpp"

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QObject>
#include <QTimer>

// Register types for signal and slots
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)

// Register type for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(const libtorrent::alert*)

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

    // View
    MainWindow view;

    // Timer which calls session.post_torrent_updates() at regular intervals
    QTimer statusUpdateTimer;

	// Listening port range
	std::pair<int, int> portRange;

	// Parameters for all torrents added
	std::vector<libtorrent::add_torrent_params> addTorrentParameters;

	// Dht routers
	std::vector<std::pair<std::string, int>> dhtRouters;

    // Torrent handles <-- why do we need this?
    //std::vector<libtorrent::torrent_handle> torrentHandles;

    // Routine for processig libtorrent alerts
    void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);
    void processTorrentRemovedAlert(libtorrent::torrent_removed_alert const * p);
    void processSaveResumeDataAlert(libtorrent::save_resume_data_alert const * p);
    void processSaveResumeDataFailedAlert(libtorrent::save_resume_data_failed_alert const * p);
    void processTorrentPausedAlert(libtorrent::torrent_paused_alert const * p);

    std::auto_ptr<std::vector<libtorrent::add_torrent_params>::iterator> findTorrentParamsFromInfoHash(const libtorrent::sha1_hash & info_hash);

    bool loadResumeDataForTorrent(QString const & save_path, QString const & file_name, std::vector<char> & resume_data) const;

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

public:

	// Constructor starting session with given state
    Controller(ControllerState state);

    // Connect controller signals to view slots
    void connecToViewSlots(MainWindow * view);

    // Callback routine called by libtorrent dispatcher routine
    void libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr);

    // Invocations of this method are queued, and dispatcher callback
    Q_INVOKABLE void processAlert(libtorrent::alert const * a);

    /*
     * Routines called by view
     */

    // Called by ...
	void saveStateToFile(const char * file);

    // Called by ...
    // REMOVE LATER, not nice to share this out.
    libtorrent::session & getSession();

    // Called by AddTorrentDialog::on_AddTorrentDialog_accepted()
    void addTorrent(libtorrent::add_torrent_params & params);

    // Called by MainWindow::on_addTorrentFilePushButton_clicked()
    void addTorrentFromTorrentFile(const QString & torrentFile);

    // Called by MainWindow::on_addMagnetLinkPushButton_clicked()
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

    /*
     * Utilities
     */

    // Called by MainWindow::
    libtorrent::torrent_handle getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash);

private slots:

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();
};

#endif
