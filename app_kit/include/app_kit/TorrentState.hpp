#ifndef JOYSTREAM_APPKIT_TORRENTSTATE_HPP
#define JOYSTREAM_APPKIT_TORRENTSTATE_HPP

#include <QJsonObject>
#include <core/Torrent.hpp>
#include <app_kit/TorrentPluginState.hpp>

namespace joystream {
namespace appkit {

class TorrentState
{
public:
    TorrentState();

    TorrentState(const core::Torrent*);

    TorrentState(const QJsonValue&);

    QJsonValue toJson() const;

    std::string savePath() const;
    std::string name() const;
    bool paused() const;
    int uploadLimit() const;
    int downloadLimit() const;
    std::vector<char> resumeData() const;
    boost::shared_ptr<libtorrent::torrent_info> metaData() const;

    TorrentPluginState pluginState() const;

private:
    QString _savePath;
    QString _name;
    bool _torrentPaused;
    int _uploadLimit;
    int _downloadLimit;

    // Bencoded torrent info
    std::vector<char> _metaData;

    // Resume data
    std::vector<char> _resumeData;

    TorrentPluginState _torrentPluginState;

    static std::vector<char> bencodeMetaData(const libtorrent::torrent_info & ti);

    static QString charVectorToBase64String(const std::vector<char> &data);

    static std::vector<char> base64StringToCharVector(const QString &base64EncodedTorrentInfo);
};

}
}

#endif // JOYSTREAM_APPKIT_TORRENTSTATE_HPP
