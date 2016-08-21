/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_LANGUAGE_HPP
#define JOYSTREAM_CLASSIC_GUI_LANGUAGE_HPP

#include <libtorrent/torrent_status.hpp>

#include <QString>

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
    class BuyingState;
}
namespace classic {
namespace gui {

class Language {

public:

    static QString toString(const libtorrent::torrent_status::state_t & state, float progress);
    static QString toString(protocol_session::SessionMode mode);
    static QString toString(const protocol_session::BuyingState & state);

};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_LANGUAGE_HPP

