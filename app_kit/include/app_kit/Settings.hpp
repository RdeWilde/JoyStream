#ifndef JOYSTREAM_APPKIT_SETTINGS_HPP
#define JOYSTREAM_APPKIT_SETTINGS_HPP

#include <QString>
#include <common/Network.hpp>

namespace joystream {
namespace appkit {

struct Settings
{
    std::string bitcoinNodeHost;
    int bitcoinNodePort;
};

} // appkit
} // joystream

#endif // JOYSTREAM_APPKIT_SETTINGS_HPP
