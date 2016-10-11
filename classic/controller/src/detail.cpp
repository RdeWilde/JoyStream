/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, October 7 2016
 */

#include <controller/detail.hpp>
#include <gui/gui.hpp>
#include <protocol_wire/protocol_wire.hpp>

#include <boost/variant/get.hpp>

namespace joystream {
namespace classic {
namespace controller {
namespace detail {

/// PeerDialogModels

ModeAnnouncedTableModelManager::ModeAnnouncedTableModelManager(const libtorrent::tcp::endpoint & endPoint,
                                                               gui::BuyerTableModel * buyerTableModel,
                                                               gui::ObserverTableModel * observerTableModel,
                                                               gui::SellerTableModel * sellerTableModel)
    : _endPoint(endPoint)
    , _buyerTableModel(buyerTableModel)
    , _observerTableModel(observerTableModel)
    , _sellerTableModel(sellerTableModel) {
}

ModeAnnouncedTableModelManager::~ModeAnnouncedTableModelManager() {

    // Remove from announced mode table models
    removeFromAnnouncedModeTables();
}

void ModeAnnouncedTableModelManager::removeFromAnnouncedModeTables() {

    if(_modeAnnouncedRowModel.is_initialized())
        removeFromTableModel(_modeAnnouncedRowModel.get());
}

void ModeAnnouncedTableModelManager::showInObserverTable() {

    if(_modeAnnouncedRowModel.is_initialized()) {

        ModeAnnouncedRowModel row = _modeAnnouncedRowModel.get();

        switch(row.which()) {

            case 0: _buyerTableModel->remove(boost::get<gui::BuyerTableRowModel *>(row)->row()); break;
            case 1: break; // nothing to do, already in observe mode
            case 2: _sellerTableModel->remove(boost::get<gui::SellerTableRowModel *>(row)->row()); break;

            default:
                assert(false);
        }

    } else
        _modeAnnouncedRowModel = _observerTableModel->add(_endPoint);
}

void ModeAnnouncedTableModelManager::showInSellerTable(const protocol_wire::SellerTerms & terms) {

    if(_modeAnnouncedRowModel.is_initialized()) {

        ModeAnnouncedRowModel row = _modeAnnouncedRowModel.get();

        switch(row.which()) {

            case 0: _buyerTableModel->remove(boost::get<gui::BuyerTableRowModel *>(row)->row()); break;
            case 1: _observerTableModel->remove(boost::get<gui::ObserverTableRowModel *>(row)->row()); break;
            case 2: boost::get<gui::SellerTableRowModel *>(row)->setTerms(terms); break;

            default:
                assert(false);
        }

    } else
        _modeAnnouncedRowModel = _sellerTableModel->add(_endPoint);
}

void ModeAnnouncedTableModelManager::showInBuyerTable(const protocol_wire::BuyerTerms & terms) {

    if(_modeAnnouncedRowModel.is_initialized()) {

        ModeAnnouncedRowModel row = _modeAnnouncedRowModel.get();

        switch(row.which()) {

            case 0: boost::get<gui::BuyerTableRowModel *>(row)->setTerms(terms); break;
            case 1: _observerTableModel->remove(boost::get<gui::ObserverTableRowModel *>(row)->row()); break;
            case 2: _sellerTableModel->remove(boost::get<gui::SellerTableRowModel *>(row)->row()); break;

            default:
                assert(false);
        }

    } else
        _modeAnnouncedRowModel = _buyerTableModel->add(_endPoint);
}

void ModeAnnouncedTableModelManager::removeFromTableModel(const ModeAnnouncedRowModel & row) {

    switch(row.which()) {

        case 0: _buyerTableModel->remove(boost::get<gui::BuyerTableRowModel *>(row)->row()); break;
        case 1: _observerTableModel->remove(boost::get<gui::ObserverTableRowModel *>(row)->row()); break;
        case 2: _sellerTableModel->remove(boost::get<gui::SellerTableRowModel *>(row)->row()); break;

        default:
            assert(false);
    }
}

}
}
}
}
