#ifndef TORRENTADDRESPONSE_HPP
#define TORRENTADDRESPONSE_HPP

#include <QObject>
#include <core/TorrentIdentifier.hpp>

namespace joystream {
namespace appkit {

class TorrentAddResponse : public QObject
{
    Q_OBJECT
public:
    enum class Error {
        NoError,
        TorrentAlreadyExists,
        TorrentRemovedBeforePluginWasAdded,
        ResumeFailed,
        LibtorrentError
    };

    explicit TorrentAddResponse(const core::TorrentIdentifier& ti)
        : _finished(false),
          _ti(ti),
          _err(Error::NoError) {
    }

    void setError(Error err) { _err = err; }
    Error error() const { return _err; }

    void setLibtorrentErrorCode(libtorrent::error_code ec) { _ec = ec; }
    libtorrent::error_code errorCode() const { return _ec; }

    void setSuccess() { _err = Error::NoError; }

    core::TorrentIdentifier torrentIdentifier() const { return _ti; }

    bool isFinished() const { return _finished; }

signals:
    void finished();

public slots:
    void finishedProcessing() {
        _finished = true;
        emit finished();
    }

private:
    bool _finished;
    libtorrent::error_code _ec;
    core::TorrentIdentifier _ti;
    Error _err;
};

}
}

#endif // TORRENTADDRESPONSE_HPP
