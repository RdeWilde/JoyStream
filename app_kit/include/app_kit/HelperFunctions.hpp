#ifndef JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP
#define JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP

#include <libtorrent/sha1_hash.hpp>
#include <core/TorrentIdentifier.hpp>

namespace joystream{
namespace appkit {
namespace util {

// Utility functions
libtorrent::sha1_hash sha1_hash_from_hex_string(const char *);
core::TorrentIdentifier* makeTorrentIdentifier(const char *);

}
}
}

#endif // JOYSTREAM_APPKIT_HELPER_FUNCTIONS_HPP
