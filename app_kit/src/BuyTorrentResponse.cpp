#include <app_kit/BuyTorrentResponse.hpp>

namespace joystream {
namespace appkit {

BuyTorrentResponse::BuyTorrentResponse(libtorrent::sha1_hash infoHash)
    : _infoHash(infoHash),
      _finished(false),
      _err(Error::NoError)
{

}

}
}
