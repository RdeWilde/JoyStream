/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#include <gui/Common.hpp>

#include <protocol_session/protocol_session.hpp>
#include <extension/extension.hpp>

#include <libtorrent/torrent_status.hpp>

#include <QObject>

namespace joystream {
namespace classic {
namespace gui {

QString Common::toString(const libtorrent::torrent_status::state_t & state, float progress) {

    switch(state) {

        #ifndef TORRENT_NO_DEPRECATE

        case libtorrent::torrent_status::queued_for_checking:

                return QObject::tr("Queued for checking");

        #else

        case libtorrent::torrent_status::unused_enum_for_backwards_compatibility:

                assert(false);
                return QObject::tr("Internal unused state");

        #endif

        case libtorrent::torrent_status::checking_files:

                return QObject::tr("Checking files") + QString::number(progress*100,'f',0).append("%");

        case libtorrent::torrent_status::downloading_metadata:

                return QObject::tr("Downloading metadata") + " " + QString::number(progress*100,'f',0).append("%");

        case libtorrent::torrent_status::downloading:

                return QObject::tr("Downloading") + " " + QString::number(progress*100,'f',0).append("%");

        case libtorrent::torrent_status::finished:

                return QObject::tr("Finished");

        case libtorrent::torrent_status::seeding:

                return QObject::tr("Seeding");

        case libtorrent::torrent_status::allocating:

                return QObject::tr("Allocating") + " " + QString::number(progress*100,'f',0).append("%");

        case libtorrent::torrent_status::checking_resume_data:

                return QObject::tr("Checking resume data") + " " + QString::number(progress*100,'f',0).append("%");

        default:
                assert(false);
    }
}

QString Common::toString(protocol_session::SessionMode mode) {

    switch(mode) {
        case protocol_session::SessionMode::buying: return QObject::tr("Buying");
        case protocol_session::SessionMode::not_set: return QObject::tr("Not set");
        case protocol_session::SessionMode::observing: return QObject::tr("Observing");
        case protocol_session::SessionMode::selling: return QObject::tr("Selling");
        default:
        assert(false);
    }
}

QString Common::toString(const protocol_session::BuyingState & state) {

    switch(state) {
        case protocol_session::BuyingState::downloading: return QObject::tr("Downloading");
        case protocol_session::BuyingState::download_completed: return QObject::tr("Downloading completed");
        case protocol_session::BuyingState::sending_invitations: return QObject::tr("Sending invitations");
        default:
        assert(false);
    }

}

QString Common::toString(const std::type_index & index) {

    /**
     * Gets state name for inner state.
     * Redo this when we finally get structure on the state of the statemachine
     */

    // not set state
    if(index == typeid(protocol_statemachine::ChooseMode))
        return "Choose mode";

    // **observe state**
    else if(index == typeid(protocol_statemachine::Observing))
        return "Observing";

    // **selling state**
    else if(index == typeid(protocol_statemachine::ReadyForInvitation))
        return "Ready for invitation";
    else if(index == typeid(protocol_statemachine::Invited))
        return "Invited";
    else if(index == typeid(protocol_statemachine::WaitingToStart))
        return "Waiting to start";
    else if(index == typeid(protocol_statemachine::ReadyForPieceRequest))
        return "Ready for piece request";
    else if(index == typeid(protocol_statemachine::LoadingPiece))
        return "Loading piece";
    else if(index == typeid(protocol_statemachine::WaitingForPayment))
        return "Waiting for payment";

    // **buying state**

    else if(index == typeid(protocol_statemachine::ReadyToInviteSeller))
        return "Ready to invite seller";
    else if(index == typeid(protocol_statemachine::WaitingForSellerToJoin))
        return "Waiting for seller to join";
    else if(index == typeid(protocol_statemachine::PreparingContract))
        return "Preparing contract";
    else if(index == typeid(protocol_statemachine::ReadyToRequestPiece))
        return "Ready to request piece";
    else if(index == typeid(protocol_statemachine::WaitingForFullPiece))
        return "Waiting for full piece";
    else if(index == typeid(protocol_statemachine::ProcessingPiece))
        return "Processing piece";
    else
        assert(false);

}

QString Common::toString(const extension::BEPSupportStatus & status) {

    switch(status) {
        case extension::BEPSupportStatus::not_supported: return QObject::tr("Not supported");
        case extension::BEPSupportStatus::supported: return QObject::tr("Supported");
        case extension::BEPSupportStatus::unknown: return QObject::tr("Unknown");
        default:
            assert(false);
    }

}

QString Common::toString(const libtorrent::tcp::endpoint & endPoint) {

    std::string endPointString = libtorrent::print_endpoint(endPoint);
    return QString::fromStdString(endPointString);
}

}
}
}
