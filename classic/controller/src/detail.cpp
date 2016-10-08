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

PeerDialogModelManager::PeerDialogModelManager(const libtorrent::tcp::endpoint & endPoint,
                                               gui::PeerTableRowModel * classicTableRowModel,
                                               gui::BuyerTableModel * buyerTableModel,
                                               gui::ObserverTableModel * observerTableModel,
                                               gui::SellerTableModel * sellerTableModel)
    : _endPoint(endPoint)
    , _classicTableRowModel(classicTableRowModel)
    , _buyerTableModel(buyerTableModel)
    , _observerTableModel(observerTableModel)
    , _sellerTableModel(sellerTableModel) {
}

void PeerDialogModelManager::noneModeAnnounced() {

    if(_modeAnnouncedRowModel.is_initialized()) {

        detail::PeerDialogModelManager::ModeAnnouncedRowModel activeRow = _modeAnnouncedRowModel.get();

        switch(activeRow.which()) {

            case 0: _buyerTableModel->remove(boost::get<gui::BuyerTableRowModel *>(activeRow)->row()); break;
            case 1: _observerTableModel->remove(boost::get<gui::ObserverTableRowModel *>(activeRow)->row()); break;
            case 2: _sellerTableModel->remove(boost::get<gui::SellerTableRowModel *>(activeRow)->row()); break;

            default:
                assert(false);
        }

    }
}

void PeerDialogModelManager::observeModeAnnounced() {

    if(_modeAnnouncedRowModel.is_initialized()) {

        detail::PeerDialogModelManager::ModeAnnouncedRowModel activeRow = _modeAnnouncedRowModel.get();

        switch(activeRow.which()) {

            case 0: _buyerTableModel->remove(boost::get<gui::BuyerTableRowModel *>(activeRow)->row()); break;
            case 1: break; // nothing to do, already in observe mode
            case 2: _sellerTableModel->remove(boost::get<gui::SellerTableRowModel *>(activeRow)->row()); break;

            default:
                assert(false);
        }

    } else
        _modeAnnouncedRowModel = _observerTableModel->add(_endPoint);
}

void PeerDialogModelManager::sellModeAnnounced(const protocol_wire::SellerTerms & terms) {

    if(_modeAnnouncedRowModel.is_initialized()) {

        detail::PeerDialogModelManager::ModeAnnouncedRowModel activeRow = _modeAnnouncedRowModel.get();

        switch(activeRow.which()) {

            case 0: _buyerTableModel->remove(boost::get<gui::BuyerTableRowModel *>(activeRow)->row()); break;
            case 1: _observerTableModel->remove(boost::get<gui::ObserverTableRowModel *>(activeRow)->row()); break;
            case 2: boost::get<gui::SellerTableRowModel *>(activeRow)->setTerms(terms); break;

            default:
                assert(false);
        }

    } else
        _modeAnnouncedRowModel = _sellerTableModel->add(_endPoint);
}

void PeerDialogModelManager::buyModeAnnounced(const protocol_wire::BuyerTerms & terms) {

    if(_modeAnnouncedRowModel.is_initialized()) {

        detail::PeerDialogModelManager::ModeAnnouncedRowModel activeRow = _modeAnnouncedRowModel.get();

        switch(activeRow.which()) {

            case 0: boost::get<gui::BuyerTableRowModel *>(activeRow)->setTerms(terms); break;
            case 1: _observerTableModel->remove(boost::get<gui::ObserverTableRowModel *>(activeRow)->row()); break;
            case 2: _sellerTableModel->remove(boost::get<gui::SellerTableRowModel *>(activeRow)->row()); break;

            default:
                assert(false);
        }

    } else
        _modeAnnouncedRowModel = _buyerTableModel->add(_endPoint);
}

}
}
}
}
