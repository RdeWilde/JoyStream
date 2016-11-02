#ifndef JOYSTREAM_APPKIT_SETTINGS_HPP
#define JOYSTREAM_APPKIT_SETTINGS_HPP

#include <QString>
#include <QJsonValue>

#include <common/Network.hpp>

namespace joystream {
namespace appkit {

struct Settings
{
    static Settings fromJson(const QJsonValue &);
    QJsonValue toJson() const;

    std::string bitcoinNodeHost;
    int bitcoinNodePort;
};

} // appkit
} // joystream

#endif // JOYSTREAM_APPKIT_SETTINGS_HPP
