/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 7 2016
 */

#include <controller/detail.hpp>
#include <gui/gui.hpp>

namespace joystream {
namespace classic {
namespace controller {
namespace detail {

/// PeerDialogModels

PeerDialogModels::PeerDialogModels(gui::PeerTableRowModel * classicTableRowModel,
                                   gui::BuyerTableModel * buyerTableModel,
                                   gui::ObserverTableModel * observerTableModel,
                                   gui::SellerTableModel * sellerTableModel)
    : _classicTableRowModel(classicTableRowModel)
    , _buyerTableModel(buyerTableModel)
    , _observerTableModel(observerTableModel)
    , _sellerTableModel(sellerTableModel) {
}


}
}
}
}
