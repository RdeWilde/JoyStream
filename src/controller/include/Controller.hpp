#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller/include/ControllerState.hpp"
#include "view/include/mainwindow.h"

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

public:

	// Constructor starting session with given state
    Controller(const ControllerState & state);

    // Connect controller signals to view slots
    void connecToViewSlots(MainWindow * view);

    // Callback routine called by libtorrent dispatcher routine
    void libtorrent_alert_dispatcher_callback(std::auto_ptr<libtorrent::alert> alertAutoPtr);

    // Invocations of this method are queued, and dispatcher callback
    Q_INVOKABLE void processAlert(libtorrent::alert const * a);

    // Stops libtorrent session, saves ...
    // Called by
    void close();

    /*
     * BELOW HERE: Routines called by view
     *
     * TURN ALL OF THESE INTO SLOTS LATER SO THAT THEY RUN IN CONTROLLER
     * THREAD
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
    void removeTorrent(const libtorrent::torrent_handle torrentHandle);

    // Called by MainWindow::
    libtorrent::torrent_handle getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash);

private slots:

    // Tells session to post updates, is signaled by timer
    void callPostTorrentUpdates();



/*
    // These signals are connected to corresponding slots on the view object,
    // and they are emitted in response to
    // 1) libtorren session alerts
    // 2)
    //

    void addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);
    void updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector);
    void updateTorrentStatus(const libtorrent::torrent_status & torrentStatus);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

*/
};

#endif
