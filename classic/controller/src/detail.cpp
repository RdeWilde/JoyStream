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

/// SessionDialogModels

SessionDialogModels::SessionDialogModels(const libtorrent::tcp::endpoint & endPoint,
                                         gui::ConnectionTableModel * sellersTableModel,
                                         gui::ConnectionTableModel * buyersTableModel)
    : _endPoint(endPoint)
    , _inTableModel(InTableModel::none)
    , _activeRow(nullptr)
    , _sellersTableModel(sellersTableModel)
    , _buyersTableModel(buyersTableModel) {
}

SessionDialogModels::~SessionDialogModels() {
    removeFromTableModel();
}

void SessionDialogModels::setInnerStateIndex(const core::CBStateMachine::InnerStateIndex &) {

    /**
     * Redo this when we finally get structure on the state of the statemachine
     */


    //new state is buying:
    // if it was in neither, then create
    // remove if it was previously in selling
    // if it was in buying, just update state

    //new state selling:
    // new state neither
    // remove if in neither

    // new stat is neither: removeFromTableModel


}

void SessionDialogModels::setPrice(quint64 price) {

    if(_activeRow == nullptr)
        throw std::runtime_error("Inappropriate machine state");

    _activeRow->setPrice(price);
}

void SessionDialogModels::setNumberOfPayments(quint64 numberOfPayments) {

    if(_activeRow == nullptr)
        throw std::runtime_error("Inappropriate machine state");

    _activeRow->setNumberOfPayments(numberOfPayments);
}

void SessionDialogModels::setFunds(quint64 funds) {

    if(_activeRow == nullptr)
        throw std::runtime_error("Inappropriate machine state");

    _activeRow->setFunds(funds);
}

void SessionDialogModels::setSettlementFee(quint64) {

    if(_activeRow == nullptr)
        throw std::runtime_error("Inappropriate machine state");

    //
}

void SessionDialogModels::setRefundLockTime(quint32 refundLockTime) {

    if(_activeRow == nullptr)
        throw std::runtime_error("Inappropriate machine state");

    _activeRow->setRefundLockTime(refundLockTime);
}

void SessionDialogModels::setAnchorChanged(const Coin::typesafeOutPoint &) {

    if(_activeRow == nullptr)
        throw std::runtime_error("Inappropriate machine state");

    //
}

void SessionDialogModels::removeFromTableModel() {

    switch(_inTableModel) {

        case InTableModel::none:
            assert(_activeRow == nullptr);
            break;
        case InTableModel::sellers_table_model:
            assert(_activeRow != nullptr);
            _sellersTableModel->remove(_activeRow->row());
            break;
        case InTableModel::buyers_table_model:
            assert(_activeRow != nullptr);
            _buyersTableModel->remove(_activeRow->row());
            break;
        default:
            assert(false);
    }
}

}
}
}
}
