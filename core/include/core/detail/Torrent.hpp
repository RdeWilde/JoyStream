/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, June 16 2016
 */

#ifndef JOYSTREAM_CORE_DETAIL_TORRENT_HPP
#define JOYSTREAM_CORE_DETAIL_TORRENT_HPP

#include <libtorrent/sha1_hash.hpp>
#include <libtorrent/torrent_handle.hpp>

namespace joystream {
namespace core {
namespace detail {

class Torrent {

public:

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


    // Status
    Status _status;

    // Add const pointer to const object of type TorrentPlugin in the future?
    // can be used to look at stuff like plugin mode etc.
    // worth looking at.

    // View model for torrent
    // We opt for shared pointer, rather than by value,
    // so that client can be given corresponding weak_ptr.
    // Giving a raw pointer can allow client to access object
    // after it is deleted. The controller::removedTorrent signal helps avoid
    // that, but it is not perfect, as client cannot be guaranteed to see signal in time.
    std::shared_ptr<viewmodel::Torrent> _model;

    // All streams for this torrent.
    // Not quite sure if multiple separate streams for one torrent
    // is necessary, if not, then remove this QSet later.
    //QSet<Stream *> _streams;
};

}
}
}

#endif // JOYSTREAM_CORE_DETAIL_TORRENT_HPP

