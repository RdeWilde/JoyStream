/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 14 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_PEERPLUGIN_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_PEERPLUGIN_HPP

#include <QObject>

namespace joystream {
namespace extension {
    enum class BEPSupportStatus;
}
namespace core {
    class PeerPlugin;
}
namespace classic {
namespace gui {
    class PeerTreeViewRow;
}
namespace controller {

class PeerPlugin : public QObject {

public:

    PeerPlugin(core::PeerPlugin * peerPlugin,
               gui::PeerTreeViewRow * peerTreeViewRow);

    void setPeerTreeViewRow(gui::PeerTreeViewRow * row);
    void unsetPeerTreeViewRow();

public slots:

    void setPeerBEP10SupportStatus(const extension::BEPSupportStatus &);

    void setPeerBitSwaprBEPSupportStatus(const extension::BEPSupportStatus &);

private:

    core::PeerPlugin * _peerPlugin;

    gui::PeerTreeViewRow * _peerTreeViewRow;
};

}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_PEERPLUGIN_HPP

