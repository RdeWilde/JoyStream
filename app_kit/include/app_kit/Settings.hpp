#ifndef JOYSTREAM_APPKIT_SETTINGS_HPP
#define JOYSTREAM_APPKIT_SETTINGS_HPP

#include <QString>
#include <common/Network.hpp>

namespace joystream {
namespace appkit {

struct Settings
{
    std::string dataDirectory;
    Coin::Network network;
    std::string host;
    int port;
};

} // appkit
} // joystream

#endif // JOYSTREAM_APPKIT_SETTINGS_HPP
