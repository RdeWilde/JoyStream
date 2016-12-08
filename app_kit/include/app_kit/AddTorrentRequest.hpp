#ifndef AddTorrentRequest_HPP
#define AddTorrentRequest_HPP

#include <core/TorrentIdentifier.hpp>

namespace joystream {
namespace appkit {

struct AddTorrentRequest
{    
    AddTorrentRequest(const core::TorrentIdentifier &ti, const std::string& savePath)
        : torrentIdentifier(ti),
          downloadLimit(0),
          uploadLimit(0),
          name(""),
          resumeData(std::vector<char>()),
          savePath(savePath),
          paused(true) {
    }

    const core::TorrentIdentifier torrentIdentifier;
    int downloadLimit;
    int uploadLimit;
    std::string name;
    std::vector<char> resumeData;
    std::string savePath;
    bool paused;
};

}
}

#endif // AddTorrentRequest_HPP
