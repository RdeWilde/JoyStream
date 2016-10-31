#include <app_kit/HelperFunctions.hpp>
#include <core/TorrentIdentifier.hpp>

#include <QFile>
#include <libtorrent/torrent_info.hpp>

namespace joystream {
namespace appkit {
namespace util {

libtorrent::sha1_hash sha1_hash_from_hex_string(const char * hex) {
  char buf[21];

  if(!libtorrent::from_hex(hex, 40, buf)){
    throw std::runtime_error("Invalid info hash string");
  }
  return libtorrent::sha1_hash(buf);
}

core::TorrentIdentifier* makeTorrentIdentifier(const std::string torrentId)
{
    core::TorrentIdentifier* ti;

    // Test if the identifier is path to a torrent file
    ti = core::TorrentIdentifier::fromTorrentFilePath(torrentId);

    if(ti)
        return ti;

    // Test if the identifier is a plain info hash string
    ti = core::TorrentIdentifier::fromHashString(torrentId);

    if(ti)
        return ti;

    // Test if the identifier is a magner link
    return core::TorrentIdentifier::fromMagnetLinkString(torrentId);
}

}
}
}
