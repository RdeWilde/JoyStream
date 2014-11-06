#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller/include/ControllerState.hpp"
#include "view/include/mainwindow.h"

#include <libtorrent/session.hpp>
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/torrent_handle.hpp>

#include <QThread>

// Register new types for signal and slot system
Q_DECLARE_METATYPE(libtorrent::sha1_hash)
Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(libtorrent::error_code)
Q_DECLARE_METATYPE(std::vector<libtorrent::torrent_status>)
Q_DECLARE_METATYPE(libtorrent::torrent_status)

class Controller : public QThread {

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

	// Listening port range
	std::pair<int, int> portRange;

	// Parameters for all torrents added
	std::vector<libtorrent::add_torrent_params> addTorrentParameters;

	// Dht routers
	std::vector<std::pair<std::string, int>> dhtRouters;

    // Torrent handles
    std::vector<libtorrent::torrent_handle> torrentHandles;

    // Main window
    MainWindow view;

    // Thread running session loop
    //boost::thread sessionLoopThread;

	// Routine for processing a single libtorrent alert
	void processAlert(const libtorrent::alert * a);

	// Routine for processig libtorrent alerts
	void processAddTorrentAlert(libtorrent::add_torrent_alert const * p);
    void processStatusUpdateAlert(libtorrent::state_update_alert const * p);

public:

	// Constructor starting session with given state
    Controller(const ControllerState & state);

    // Start session loop thread and show view
    void begin();
	
	/*
	* Service loop for libtorrent alerts, calling this
	* causes infinite looping, closing client requires
	* issuing a command.
	*/
    void run();

	// Saves to file
	void saveStateToFile(const char * file);

	// Returns session, for control by view
	libtorrent::session getSession();

    // Called by AddTorrentDialog::on_AddTorrentDialog_accepted()
    void addTorrent(libtorrent::add_torrent_params & params);

    // Called by MainWindow::on_addTorrentFilePushButton_clicked()
    void addTorrentFromTorrentFile(const QString & torrentFile);

    // Called by MainWindow::on_addMagnetLinkPushButton_clicked()
    void addTorrentFromMagnetLink(const QString & magnetLink);

    // Called by MainWindow::
    libtorrent::torrent_handle & Controller::getTorrentHandleFromInfoHash(const libtorrent::sha1_hash & info_hash);

signals:
    /*
     * These signals are connected to corresponding slots on the view object,
     * and they are emitted in response to
     * 1) libtorren session alerts
     * 2)
    */

    void addTorrent(const libtorrent::sha1_hash & info_hash, const std::string & torrentName, int totalSize);
    void addTorrentFailed(const std::string & name, const libtorrent::sha1_hash & info_has, const libtorrent::error_code & ec);
    void updateTorrentStatus(const std::vector<libtorrent::torrent_status> & torrentStatusVector);
    void updateTorrentStatus(const libtorrent::torrent_status & torrentStatus);
    void removeTorrent(const libtorrent::sha1_hash & info_hash);

};

#endif
