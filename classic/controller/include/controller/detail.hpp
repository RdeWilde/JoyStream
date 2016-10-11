/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 7 2016
 */

#ifndef JOYSTREAM_CLASSIC_CONTROLLER_DETAIL_HPP
#define JOYSTREAM_CLASSIC_CONTROLLER_DETAIL_HPP

#include <core/core.hpp> // InnerStateIndex
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
    class PeerTableModel;
    class PeerTableRowModel;
    class BuyerTableModel;
    class BuyerTableRowModel;
    class ObserverTableModel;
    class ObserverTableRowModel;
    class SellerTableModel;
    class SellerTableRowModel;
    class ConnectionTableRowModel;
    class ConnectionTableModel;
}
namespace controller {
namespace detail {

struct ModeAnnouncedTableModelManager {

    ModeAnnouncedTableModelManager(const libtorrent::tcp::endpoint & endPoint,
                                   gui::BuyerTableModel * buyerTableModel,
                                   gui::ObserverTableModel * observerTableModel,
                                   gui::SellerTableModel * sellerTableModel);

    ~ModeAnnouncedTableModelManager();

    void removeFromAnnouncedModeTables();

    void showInObserverTable();

    void showInSellerTable(const protocol_wire::SellerTerms & terms);

    void showInBuyerTable(const protocol_wire::BuyerTerms & terms);

    // Peer endpoint
    libtorrent::tcp::endpoint _endPoint;

    typedef boost::variant<gui::BuyerTableRowModel *,
                           gui::ObserverTableRowModel *,
                           gui::SellerTableRowModel *> ModeAnnouncedRowModel;

    // Model for connection view of peer
    boost::optional<ModeAnnouncedRowModel> _modeAnnouncedRowModel;

    // Table view models are required to manage presence of connection
    // depending on peer terms at any given time
    gui::BuyerTableModel * _buyerTableModel;
    gui::ObserverTableModel * _observerTableModel;
    gui::SellerTableModel * _sellerTableModel;

private:

    void removeFromTableModel(const ModeAnnouncedRowModel & row);
};

struct SessionDialogModels {

    // The table model in which this peer is currently
    // shown. Since we are using same underlying type for thi
    enum InTableModel {
        none,
        sellers_table_model,
        buyers_table_model
    };

    SessionDialogModels(const libtorrent::tcp::endpoint & endPoint,
                        gui::ConnectionTableModel * sellersTableModel,
                        gui::ConnectionTableModel * buyersTableModel);

    ~SessionDialogModels();

    void setInnerStateIndex(const core::CBStateMachine::InnerStateIndex & index);

    // Size of single payment
    void setPrice(quint64);

    // Number of payments made
    void setNumberOfPayments(quint64);

    // Funds allocated to output
    void setFunds(quint64);

    // Settlement fee
    void setSettlementFee(quint64);

    // Lock time of refund, received in
    void setRefundLockTime(quint32);

    // Anchor for channel in contract transaction
    void setAnchorChanged(const Coin::typesafeOutPoint &);

    // Peer endpoint
    libtorrent::tcp::endpoint _endPoint;

    InTableModel _inTableModel;

    gui::ConnectionTableRowModel * _activeRow;

    gui::ConnectionTableModel * _sellersTableModel,
                              * _buyersTableModel;

    void removeFromTableModel();

};

}
}
}
}

#endif // JOYSTREAM_CLASSIC_CONTROLLER_DETAIL_HPP

