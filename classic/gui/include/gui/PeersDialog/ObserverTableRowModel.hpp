/**
 * Copyright (C) JoyStream - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Bedeho Mender <bedeho.mender@gmail.com>, September 18 2016
 */

#ifndef JOYSTREAM_CLASSIC_GUI_OBSERVERTABLEROWMODEL_HPP
#define JOYSTREAM_CLASSIC_GUI_OBSERVERTABLEROWMODEL_HPP

#include <libtorrent/socket.hpp>
#include <QObject>

class BitcoinDisplaySettings;

class QStandardItem;

namespace joystream {
namespace classic {
namespace gui {

class ObserverTableRowModel : public QObject {

    Q_OBJECT

public:

    ObserverTableRowModel(QStandardItem * hostItem,
                          const BitcoinDisplaySettings * settings);

public slots:

    void setHost(const libtorrent::tcp::endpoint & endPoint);

public:

    int row() const noexcept;

private:

    QStandardItem * _hostItem;

    const BitcoinDisplaySettings * _settings;

};

}
}
}


#endif // JOYSTREAM_CLASSIC_GUI_OBSERVERTABLEROWMODEL_HPP
