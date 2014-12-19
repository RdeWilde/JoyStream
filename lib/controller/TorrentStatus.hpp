#ifndef TORRENT_STATUS_HPP
#define TORRENT_STATUS_HPP

#include <vector>

// State of torrents, only short term within session
class TorrentStatus {

public:

    // Resume data provided so far
    std::vector<char> _resumeData;

    // When libtorrent issues a torrent_checked_alert, then this torrent needs
    // a torrent plugin configuration.
    // This will only be true for torrents added during this session, not torrents loaded from previos sessions.
    bool _needsTorrentPluginConfigurationAfterTorrentCheckedAlert;

    // Constructor
    TorrentStatus(const std::vector<char> & resumeData, bool needsTorrentPluginConfigurationAfterTorrentCheckedAlert);

};

#endif // TORRENT_STATUS_HPP
