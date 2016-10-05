/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, August 19 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_COMMON_HPP
#define JOYSTREAM_CLASSIC_GUI_COMMON_HPP

#include <libtorrent/torrent_status.hpp>

#include <QString>

#include <typeindex>

#include <QStandardItemModel>

namespace joystream {
namespace protocol_session {
    enum class SessionMode;
    enum class BuyingState;
}
namespace extension {
    enum class BEPSupportStatus;
}
namespace classic {
namespace gui {

class Common {

public:

    static QString toString(const libtorrent::torrent_status::state_t & state, float progress);
    static QString toString(protocol_session::SessionMode mode);
    static QString toString(const protocol_session::BuyingState & state);
    static QString toString(const std::type_index & index);
    static QString toString(const extension::BEPSupportStatus & status);
    static QString toString(const libtorrent::tcp::endpoint & endPoint);

    // move later
    template<class T>
    static T getUserRoleDataFromTableModel(const QStandardItemModel & model, int row, int col, int roleOffset = 0) {

        if(model.rowCount() <= row)
            throw std::runtime_error("Invalid row index provided");

        // Return user role data from first (name) column
        QVariant var = model.item(row, col)->data(Qt::UserRole + roleOffset);

        assert(var.canConvert<T>());

        return var.value<T>();
    }
};

}
}
}

#endif // JOYSTREAM_CLASSIC_GUI_COMMON_HPP

