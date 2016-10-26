#include <app_kit/HelperFunctions.hpp>

#include <QFile>
#include <boost/make_shared.hpp>
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

core::TorrentIdentifier* makeTorrentIdentifier(const char *str)
{
    libtorrent::error_code ec;
    boost::shared_ptr<libtorrent::torrent_info> ti;

    // Is it a path to a torrent file ?
    if(QFile::exists(str)) {
        ti = boost::make_shared<libtorrent::torrent_info>(std::string(str), boost::ref(ec), 0);
        if (ec) {
            std::cerr << ec.message().c_str() << std::endl;
            return nullptr;
        } else {
            return new joystream::core::TorrentIdentifier(ti);
        }
    }

    // Is it an infohash string
    if(strlen(str) == 40) {
        try {
            ti = boost::make_shared<libtorrent::torrent_info>(sha1_hash_from_hex_string(str), 0);
            return new joystream::core::TorrentIdentifier(ti);
        } catch(std::exception &e) {
            std::cerr << "Failed to parse info hash: " << e.what() << std::endl;
        }
    }

    // Is it a magnet link ?
    try {
        auto magnetLink = joystream::core::MagnetLink::fromURI(str);
        return new joystream::core::TorrentIdentifier(magnetLink);
    } catch (std::exception &e) {
        std::cerr << "Failed to parse magnet link" << std::endl;
    }

    return nullptr;
}

}
}
}
