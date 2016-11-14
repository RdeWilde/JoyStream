#include <app_kit/SavedTorrentParameters.hpp>
#include <libtorrent/create_torrent.hpp>
#include <app_kit/HelperFunctions.hpp>

namespace joystream {
namespace appkit {

SavedTorrentParameters::SavedTorrentParameters()
{

}

SavedTorrentParameters::SavedTorrentParameters(const core::Torrent *t)
    : _infoHash(t->infoHash()),
      _savePath(QString::fromStdString(t->savePath())),
      _name(QString::fromStdString(t->name())),
      _torrentPaused(t->isPaused()),
      _uploadLimit(t->uploadLimit()),
      _downloadLimit(t->downloadLimit()),
      _resumeData(t->resumeData())
{

    auto ti = t->metaData().lock();

    if(ti && ti->is_valid()) {
        _metaData = bencodeMetaData(*ti);
    }

    if(t->torrentPluginSet())
        _torrentSessionParameters = SavedSessionParameters(t->torrentPlugin());
}

SavedTorrentParameters::SavedTorrentParameters(const QJsonValue &value) {
    if(!value.isObject())
        throw std::runtime_error("expecting object json value");

    QJsonObject state = value.toObject();

    _infoHash = util::jsonToSha1Hash(state["infoHash"]);
    _savePath = state["savePath"].toString();
    _name = state["name"].toString();
    _torrentPaused = state["paused"].toBool();
    _uploadLimit = state["uploadLimit"].toInt();
    _downloadLimit = state["downloadLimit"].toInt();
    _resumeData = base64StringToCharVector(state["resumeData"].toString());
    _metaData = base64StringToCharVector(state["metaData"].toString());
    _torrentSessionParameters = SavedSessionParameters(state["pluginState"]);
}

libtorrent::sha1_hash SavedTorrentParameters::infoHash() const {
    return _infoHash;
}
std::string SavedTorrentParameters::savePath() const {
    return _savePath.toStdString();
}

std::string SavedTorrentParameters::name() const {
    return _name.toStdString();
}

bool SavedTorrentParameters::paused() const {
    return _torrentPaused;
}

int SavedTorrentParameters::uploadLimit() const {
    return _uploadLimit;
}

int SavedTorrentParameters::downloadLimit() const {
    return _downloadLimit;
}

std::vector<char> SavedTorrentParameters::resumeData() const {
    return _resumeData;
}

boost::shared_ptr<libtorrent::torrent_info> SavedTorrentParameters::metaData() const {
    libtorrent::bdecode_node decodedMetaData;
    libtorrent::error_code ec;

    libtorrent::bdecode(_metaData.data(), _metaData.data() + _metaData.size(), decodedMetaData, ec);

    if(ec) {
        return boost::shared_ptr<libtorrent::torrent_info>(nullptr);
    }

    return boost::shared_ptr<libtorrent::torrent_info>(new libtorrent::torrent_info(decodedMetaData));
}

SavedSessionParameters SavedTorrentParameters::sessionParameters() const {
    return _torrentSessionParameters;
}

QJsonValue SavedTorrentParameters::toJson() const {
    QJsonObject state;

    state["infoHash"] = util::sha1HashToJson(_infoHash);
    state["savePath"] = QJsonValue(_savePath);
    state["name"] = QJsonValue(_name);
    state["paused"] = QJsonValue(_torrentPaused);
    state["uploadLimit"] = QJsonValue(_uploadLimit);
    state["downloadLimit"] = QJsonValue(_downloadLimit);
    state["resumeData"] = QJsonValue(charVectorToBase64String(_resumeData));
    state["metaData"] = QJsonValue(charVectorToBase64String(_metaData));
    state["pluginState"] = _torrentSessionParameters.toJson();
    return state;
}

std::vector<char> SavedTorrentParameters::bencodeMetaData(const libtorrent::torrent_info & ti) {
    libtorrent::create_torrent ct(ti);

    auto metadata = ct.generate();

    std::vector<char> encoded;

    if(metadata.type() != libtorrent::entry::undefined_t) {
        libtorrent::bencode(std::back_inserter(encoded), metadata);
    }

    return encoded;
}

QString SavedTorrentParameters::charVectorToBase64String(const std::vector<char> &vec) {
    QByteArray data(vec.data(), vec.size());
    return data.toBase64();
}

std::vector<char> SavedTorrentParameters::base64StringToCharVector(const QString &base64EncodedTorrentInfo) {
    QByteArray data = QByteArray::fromBase64(base64EncodedTorrentInfo.toLocal8Bit());
    return std::vector<char>(data.begin(), data.end());
}

}
}
