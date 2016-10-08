/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 7 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_DETAIL_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_DETAIL_HPP

#include <libtorrent/socket.hpp>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

namespace joystream {
namespace protocol_wire {
    class SellerTerms;
    class BuyerTerms;
}
namespace classic {
namespace gui {
    class PeerTableRowModel;
    class BuyerTableModel;
    class BuyerTableRowModel;
    class ObserverTableModel;
    class ObserverTableRowModel;
    class SellerTableModel;
    class SellerTableRowModel;
}
namespace controller {
namespace detail {

struct PeerDialogModelManager {

    PeerDialogModelManager(const libtorrent::tcp::endpoint & endPoint,
                           gui::PeerTableRowModel * classicTableRowModel,
                           gui::BuyerTableModel * buyerTableModel,
                           gui::ObserverTableModel * observerTableModel,
                           gui::SellerTableModel * sellerTableModel);

    /// Mode announcement handlers

    void noneModeAnnounced();

    void observeModeAnnounced();

    void sellModeAnnounced(const protocol_wire::SellerTerms & terms);

    void buyModeAnnounced(const protocol_wire::BuyerTerms & terms);

    /// Members

    libtorrent::tcp::endpoint _endPoint;

    // Model for classic view of this peer
    gui::PeerTableRowModel * _classicTableRowModel;

    // this has to be moved
    typedef boost::variant<gui::BuyerTableRowModel *,
                           gui::ObserverTableRowModel *,
                           gui::SellerTableRowModel *> ModeAnnouncedRowModel;

    // Model for connection view of peer
    boost::optional<ModeAnnouncedRowModel> _modeAnnouncedRowModel;

    // Table View Models
    // Are required to manage presence of connection depending on peer terms at any given time
    gui::BuyerTableModel * _buyerTableModel;
    gui::ObserverTableModel * _observerTableModel;
    gui::SellerTableModel * _sellerTableModel;
};

struct SessionDialogModels {

};

}
}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_DETAIL_HPP

