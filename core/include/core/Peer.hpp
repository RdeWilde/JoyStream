/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, July 11 2016
 */

#ifndef JOYSTREAM_CORE_PEER_HPP
#define JOYSTREAM_CORE_PEER_HPP

#include <libtorrent/peer_info.hpp>
#include <QObject>

namespace joystream {
namespace core {

class Peer : public QObject {

    Q_OBJECT

private:

    Peer(const libtorrent::peer_info &);

public:

    libtorrent::tcp::endpoint endPoint() const noexcept;

    std::string client();

    boost::int64_t total_download() const noexcept;

    boost::int64_t total_upload() const noexcept;

    int payload_up_speed() const noexcept;

    int payload_down_speed() const noexcept;

signals:

    void clientChanged(std::string & client);

    void total_downloadChanged(boost::int64_t);

    void total_upload(boost::int64_t);

    void payload_up_speed(int);

    void payload_down_speed(int);

private:

    friend class Torrent;

    void update(const libtorrent::peer_info & peerInformation);

    libtorrent::peer_info _peerInformation;
};

}
}

#endif // JOYSTREAM_CORE_PEER_HPP
